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
    char wiersz[MAX_LINE];
    while (fgets(wiersz, sizeof(wiersz), plik)){
        printf("%s \n",wiersz);
    }
    fclose(plik);
    return 0;
}
