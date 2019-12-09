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
                if(var==0) tab[*i]=1;
                else tab[*i]=3;
                (*line)++;
            }
            else if(c=='-') {
                if(var==0) tab[*i]=0;
                else tab[*i]=2;
                (*line)++;
            }
            else if(c=='{') {
                if(var==0) {
                    tab[*i]=4;
                    (*i)++;
                    (*line)++;
                    tab=analize(tab, i, size, cell, c, line);
                    (*line)--;
                    tab[*i]=6;
                    (*i)++;
                    (*line)++;
                    c=load();
                    tab=analize(tab, i, size, cell, c, line);
                    (*i)--;
                    (*line)--;
                }
                else {
                    tab[*i]=5;
                    (*i)++;
                    (*line)++;
                    tab=analize(tab, i, size, cell, c, line);
                    tab[*i]=6;
                    (*i)++;
                    (*line)++;
                    c=load();
                    tab=analize(tab, i, size, cell, c, line);
                    (*i)--;
                }
            }
            else if(c>='A' && c<='Z') {
                tab[*i]=7;
                (*i)++;
                (*line)++;
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

int *compile(int *size, int *line, int *stack) {
    int *tab=NULL;
    int i=0;
    int c=0;
    int cell=0;
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
    int j=0;
    int ja=0;
    int k=1;
    int ka=1;
    int stack[27];
    int line=1;
    int size=0;
    int pom=0;
    for(j=0; j<27; j++) stack[j]=-1;
    int *comp=compile(&size, &line, stack);
    printf("6 %d\n", stack[26]);
    for(j=0; j<size; j++) {
        if(comp[j]>=100 && comp[j]<200) {
            pom=comp[j];
            j++;
            if(comp[j]==0 || comp[j]==1) {
                while(comp[j]==0 || comp[j]==1) {
                    printf("%d %d\n", comp[j], pom-100);
                    j++;
                    k++;
                }
                j--;
            }
            else if(comp[j]==4) {
                ka=k;
                ja=j;
                while(comp[ja]!=6) {
                    if(comp[ja]>=0 && comp[ja]<=9) ka++;
                    ja++;
                }
                printf("4 %d %d\n", ka+1, pom-100);
                k++;
            }
        }
        else if(comp[j]==50) {
            j++;
            if(comp[j]==2 || comp[j]==3) {
                while(comp[j]==2 || comp[j]==3) {
                    printf("%d\n", comp[j]);
                    j++;
                    k++;
                }
                j--;
            }
            else if(comp[j]==5) {
                ka=k;
                ja=j;
                while(comp[ja]!=6) {
                    if(comp[ja]>=0 && comp[ja]<=9) ka++;
                    ja++;
                }
                printf("5 %d\n", ka+1);
                k++;
            }
        }
        else if(comp[j]==6) {
            ka=k;
            ja=j;
            while(comp[ja]!=8) {
                if(comp[ja]>=0 && comp[ja]<=9) ka++;
                ja++;
            }
            printf("6 %d\n", ka);
            k++;
        }
        else if(comp[j]==8 && k!=line) {
            printf("8\n");
            k++;
        }
        else if(comp[j]==7) {
            j++;
            printf("7 %d\n", stack[comp[j]*(-1)-265]);
            k++;
        }
    }
    printf("9\n");
    free(comp);
}

int main(void)
{
    read();
    return 0;
}
