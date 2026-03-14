#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
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
