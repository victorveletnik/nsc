#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 4096
//Używam stringów, a więc strlen i '\0', bo nie muszę męczyć się z całą tablicą dzięki temu
int czy_srednik(const char *wiersz){
    //Kopiuję bo wymusza to const w parametrze, a używam go dla przejrzystości i bezpieczeństwa
    char kopia [MAX_LINE];
    //-1 pozwala zachować miejsce dla '\0'
    strncpy(kopia, wiersz, MAX_LINE-1);
    kopia[MAX_LINE-1]='\0';
    //Usuwam białe znaki z końca wiersza
    int i=strlen(kopia)-1;
    while (i>=0 && isspace(kopia[i]))
    {
        kopia[i]='\0';
        i--;
    }
    //Usuwam białe znaki z początku wiersza
    int start = 0;
    while (isspace(kopia[start])){
         start++;
    }
    if(strlen(kopia+start)==0) return 0;    
    char ostatni=kopia[strlen(kopia+start)-1];
    //Zakończone
    if (ostatni == ';' || ostatni == '{' ||ostatni == '}' ||ostatni == ',' ||ostatni == ':' ) return 0;
    //Dyrektywy prepocesora
    if (kopia[start] == '#') return 0;
    //Komentarze
    if (kopia[start] == '/' && kopia[start+1] == '/') return 0;
    if (kopia[start] == '/' && kopia[start+1] == '*') return 0;
    if (kopia[start] == '*') return 0;
    //Słowa
    const char *keywords[]={
        "if", "else", "for", "while", "do", "switch", "case", "default",
    "struct", "union", "enum"
    };
    int n = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < n; i++){
        int dl=strlen(keywords[i]);
        if (strncmp(kopia + start, keywords[i], dl) == 0) {
            char nastepnik = kopia[start + dl];
            if (nastepnik == '\0' || isspace(nastepnik) || nastepnik == '(')
                return 0;
        }
    }
    return 1;
    }

int main (int argc, char *argv[]){
    if(argc<2){
        fprintf(stderr, "Wrong syntax. Usage: nsc <filename>.nsc \n");
        return 1;
    }
    FILE *plik=fopen(argv[1],"r");
    if(!plik){
        fprintf(stderr, "Reading failed\n");
        return 1;
    }
    //Bezpiecznie zpisuję nazwe pliku, który chcę zmodyfikować
    char output[MAX_LINE];
    strncpy(output, argv[1], MAX_LINE - 1);
    //Ochrona przed przepełnieniem bufora
    output[MAX_LINE - 1] = '\0'; 
    char *kropka = strrchr(output, '.');
    if (kropka) *kropka = '\0';
    //Tworze tymczasowy plik
    FILE *tmp = fopen("/tmp/nsc_temp.c", "w");
    if (!tmp) {
        fprintf(stderr, "Creation of temp file failed\n");
        fclose(plik);
        return 1;
    }
    //do którego będę wpisywał poprawiony kod...
    char wiersz[MAX_LINE];
    while (fgets(wiersz, sizeof(wiersz), plik)) {
        if (czy_srednik(wiersz)) {
            wiersz[strcspn(wiersz, "\n")] = '\0';
            fprintf(tmp, "%s;\n", wiersz);
        } else {
            fprintf(tmp, "%s", wiersz);
        }
    }
    fclose(tmp);
    fclose(plik);
    //i który następnie skompiluję...
    char bash[MAX_LINE*2];
    snprintf(bash, sizeof(bash), "gcc /tmp/nsc_temp.c -o %s", output);
    int wynik = system(bash);
    //a pod koniec usuwam, aby zachować czystość
    remove("/tmp/nsc_temp.c");
    if(wynik){
        fprintf(stderr, "Compilation failed \n");
        return 1;
    }
    printf("Compiled. Run with: ./%s \n",output);
    return 0;
}
