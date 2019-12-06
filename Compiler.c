/**
 * Zadanie zaliczeniowe 2 - kompilator
 * Michał Skwarek
 * ms418426
 * gr. 4 - MD
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Funkcja wczytujaca znaczace znaki programu, ktora przekazuje je
 * do dynamicznie alokowanej tablicy charow
 *
 * Argumenty:
 * *size - rozmiar powstalej tablicy
 */

char *load(int *size) {
    char *tab=NULL;
    int cell=0;
    int i=0;
    int j=0;
    int c=0;
    for(i=0; (c=getchar())!=EOF; i++) {
        if(j>=cell) {
            cell+=sizeof(char);
            tab=realloc(tab, cell);
        }
        if(c==';') {
            while((c=getchar())!='\n') i++;
        }
        if(c=='\n' || c=='\t' || c==' ');
        else {
            tab[j]=c;
            j++;
        }
    }
    *size=j;
    return tab;
}

int main(void)
{
    int size;
    char *program=load(&size);
    for(int i=0; i<size; i++) printf("%c", program[i]);
    free(program);
    return 0;
}
