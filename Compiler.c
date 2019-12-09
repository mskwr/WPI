/**
 * Zadanie zaliczeniowe 2 - kompilator
 * Michał Skwarek
 * ms418426
 * gr. 4 - MD
 */

#include <stdio.h>
#include <stdlib.h>

int load() {
    int c=0;
    int var=0;
    while(var==0 && (c=getchar())!=EOF) {
        if (c==';') {
            while((c=getchar())!='\n');
        }
        else if(c!='\n' && c!='\t' && c!=' ') var=1;
    }
    return c;
}

int *analize(int *tab, int *i, int *size, int *cell) {
    int c=0;
    int var=0;
    if((c=load())=='{') {
        while((c=load())!='}') {
            if(i==cell) {
                *cell=1+*cell*2;;
                tab=realloc(tab, *cell * sizeof *tab);
            }
            if(c>='a' && c<='z') {
                var=0;
                tab[*i]=c+3;
            }
            else if(c=='$') {
                var=1;
                tab[*i]=50;
            }
            else if(c=='+') {
                if(var==0) tab[*i]=1;
                else tab[*i]=3;
            }
            else if(c=='-') {
                if(var==0) tab[*i]=0;
                else tab[*i]=2;
            }
            else if(c=='{') {
                if(var==0) {
                    tab[*i]=4;
                    i++;
                    tab=analize(tab, i, size, cell);
                    tab[*i]=6;
                    tab=analize(tab, i, size, cell);
                }
                else {
                    tab[*i]=5;
                    i++;
                    tab=analize(tab, i, size, cell);
                    tab[*i]=6;
                    tab=analize(tab, i, size, cell);
                }
            }
            else if(c>='A' && c<='Z') {
                tab[*i]=7;
                i++;
                tab[*i]=-200-c;
            }
            i++;
            tab[*i]=8;
            i++;
            *size=*i;
        }
    }
    return tab;
}

int *compile(int *size) {
    int *tab=NULL;
    int i=0;
    int c=0;
    int cell=0;
    for(i=0; (c=load())!=EOF; i++) {
        if(i==cell) {
            cell=1+cell*2;;
            tab=realloc(tab, cell * sizeof *tab);
        }
        if(c>='A' && c<='Z') {
            tab[i]=-100-c;
            i++;
            tab=analize(tab, &i, size, &cell);
        }
        else {
            tab[i]=-1000;
            i++;
            tab=analize(tab, &i, size, &cell);
        }
    }
    return tab;
}

int main(void)
{
    int size=0;
    int *compiler=compile(&size);
    for(int i=0; i<size; i++) printf("%d", compiler[i]);
    free(compiler);
    return 0;
}
