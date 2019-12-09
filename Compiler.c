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

int *analize(int *tab, int *i, int *size, int *cell, int c, int *line) {
    int var=0;
    if(c=='{') {
        while((c=load())!='}') {
            if(*i>=*cell) {
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
                if(var==0) {
                    tab[*i]=1;
                    (*line)++;
                }
                else {
                    tab[*i]=3;
                    (*line)++;
                }
            }
            else if(c=='-') {
                if(var==0) {
                    tab[*i]=0;
                    (*line)++;
                }
                else {
                    tab[*i]=2;
                    (*line)++;
                }
            }
            else if(c=='{') {
                if(var==0) {
                    tab[*i]=4;
                    (*i)++;
                    (*line)++;
                    tab=analize(tab, i, size, cell, c, line);
                    tab[*i]=6;
                    (*line)++;
                    (*i)++;
                    c=load();
                    tab=analize(tab, i, size, cell, c, line);
                    (*i)--;
                }
                else {
                    tab[*i]=5;
                    (*line)++;
                    (*i)++;
                    tab=analize(tab, i, size, cell, c, line);
                    tab[*i]=6;
                    (*line)++;
                    (*i)++;
                    c=load();
                    tab=analize(tab, i, size, cell, c, line);
                    (*i)--;
                }
            }
            else if(c>='A' && c<='Z') {
                tab[*i]=7;
                (*line)++;
                (*i)++;
                tab[*i]=-200-c;
            }
            (*i)++;
        }
        tab[*i]=8;
        (*line)++;
        *size=*i;
    }
    return tab;
}

int *compile(int *size, int *line) {
    int *tab=NULL;
    int stack[27]={0};
    int i=0;
    int c=0;
    int cell=0;
    for(i=0; i<27; i++) stack[i]=-1;
    for(i=0; (c=load())!=EOF; i++) {
        if(i>=cell) {
            cell=1+cell*2;;
            tab=realloc(tab, cell * sizeof *tab);
        }
        if(c>='A' && c<='Z') {
            stack[c-65]=*line;
            tab[i]=-100-c;
            i++;
            c=load();
            tab=analize(tab, &i, size, &cell, c, line);
        }
        else {
            stack[26]=*line;
            tab[i]=-1000;
            i++;
            tab=analize(tab, &i, size, &cell, c, line);
        }
    }
    return tab;
}

void read() {
    int i=0;
    int line=1;
    int size=0;
    int *compiler=compile(&size, &line);
    for(i=0; i<=size; i++) printf("%d", compiler[i]);
    free(compiler);
}

int main(void)
{
    read();
    return 0;
}
