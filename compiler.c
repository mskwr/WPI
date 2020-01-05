/**
 * Zadanie zaliczeniowe 2 - kompilator
 * Michał Skwarek
 * ms418426
 * gr. 4 - MD
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * Procedura wczytujaca kolejny znaczacy znak programu
 *
 * Wynik:
 * c - kolejny znaczacy znak programu
 */

int load() {
    int c=0;
    int var=0;  // zmienna okreslajaca czy znak jest pusty
    while(var==0 && (c=getchar())!=EOF) {
        if (c==';') {
            while((c=getchar())!='\n');
        }
        else if(c!='\n' && c!='\t' && c!=' ') var=1;
    }
    return c;
}

/**
 * Funkcja analizujaca jedna funkcje kodu (rowniez okreslona rekurencyjnie)
 * ktora przetwarza znaczace znaki na odpowiednie cyfry i wpisuje je do tablicy
 *
 * Argumenty:
 * *tab - tablica do ktorej wpisywane sa znaki
 * *i - indeks przekazywanej tablicy, aby mozna bylo uzywac tylko jednej
 * *size - rozmiar otrzymanej tablicy
 * *cell - liczba dostepnych miejsc w tablicy (aby realokowac pamiec)
 * *line - obecna linijka kodu maszynowego
 *
 * Wynik:
 * tab - tablica wypelniona odpowiednimi cyframi przetworzonymi z kodu
 *       calej jednej funkcji
 */

int *analize(int *tab, int *i, int *size, int *cell, int c, int *line) {
    int var=0;  // zmienna okreslajaca czy ostatnim znakiem byl znak specjalny
    if(c=='{') {
        while((c=load())!='}') {
            if(*i>=*cell) {
                *cell=1+*cell*2;
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
            else if(c=='{') {  // rekurencyjne wywolanie w przypadku
                if(var==0) {   // zagniezdzonych instrukcji
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
                    tab[*i]=800;
                }
                else {         // przypadek znaku specjalnego
                    tab[*i]=5;
                    (*i)++;
                    (*line)++;
                    tab=analize(tab, i, size, cell, c, line);
                    (*line)--;
                    tab[*i]=6;
                    (*i)++;
                    (*line)++;
                    c=load();
                    tab=analize(tab, i, size, cell, c, line);
                    tab[*i]=800;
                }
                (*line)--;
            }
            else if(c>='A' && c<='Z') {  // odwolanie sie do innej funkcji
                tab[*i]=7;
                (*i)++;
                if(*i>=*cell) {
                    *cell=1+*cell*2;;
                    tab=realloc(tab, *cell * sizeof *tab);
                }
                tab[*i]=-200-c;
                (*line)++;
            }
            (*i)++;
        }
        if(*i>=*cell) {
            *cell=1+*cell*2;;
            tab=realloc(tab, *cell * sizeof *tab);
        }
        tab[*i]=8;
        (*line)++;
        *size=*i;
    }
    return tab;
}

/**
 * Funkcja wpisujaca do tablicy wszystkie przeanalizowane instrukcje w
 * odpowiedniej kolejnosci oraz zapamietujaca adresy tych instrukcji
 *
 * Argumenty:
 * *size - rozmiar tablicy
 * *line - obecny adres kodu maszynowego
 * *stack - tablica do ktorej wpisywane sa adresy wszystkich instrukcji
 *
 * Wynik:
 * tab - tablica wypelniona odpowiednimi cyframi przetworzonymi z kodu
 *       calego programu
 */

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
            stack[c-65]=*line;  // ze wzgledu na kod ASCII znakow
            tab[i]=-100-c;      // oznaczajacych instrukcje
            i++;
            c=load();
            tab=analize(tab, &i, size, &cell, c, line);
        }
        else {
            stack[26]=*line;  // ostatnie dodatkowe miejsce zarezerwowane
            tab[i]=-1000;     // dla funkcji glownej
            i++;
            tab=analize(tab, &i, size, &cell, c, line);
        }
    }
    return tab;
}

/**
 * Procedura przetwarzajaca kompletny kod programu w tablicy i na jego
 * podstawie wypisujaca kod maszynowy do interpretora
 */

void read() {
    int j=0, ja=0;  // ja to kopia indeksu j potrzebna do lokalizacji adresow
    int size=0;     // rozmiar tablicy
    int pom=0;      // zmienna pomocnicza do zapamietania danego znaku  
    int bracket=0;  // zmienna sprawdzajaca w ktorej warstwie zagniezdzenia
    int k=1, ka=1;  // jest program
    int stack[27];
    int line=1;
    for(j=0; j<27; j++) stack[j]=-1;
    int *comp=compile(&size, &line, stack);
    printf("6 %d\n", stack[26]);
    for(j=0; j<size; j++) {
        if(comp[j]>=100 && comp[j]<200) {  // odczytanie numeru stosu
            pom=comp[j];
            j++;
            if(comp[j]==0 || comp[j]==1) {
                while(comp[j]==0 || comp[j]==1) {
                    printf("%d %d\n", comp[j], pom-100);
                    j++; k++;
                }
                j--;
            }
            else if(comp[j]==4) {  // instrukcja wyboru
                ka=k;
                ja=j+1;
                bracket=1;
                while(bracket!=0) {  // przypadek zagniezdzenia
                    if(comp[ja]>=0 && comp[ja]<=9) {
                        if(comp[ja]==6) bracket--;
                        else if(comp[ja]==4 || comp[ja]==5) bracket++;
                        ka++;
                    }
                    ja++;
                }
                printf("4 %d %d\n", ka+1, pom-100);
                k++;
            }
        }
        else if(comp[j]==50) {  // przypadek znaku specjalnego
            j++;
            if(comp[j]==2 || comp[j]==3) {
                while(comp[j]==2 || comp[j]==3) {
                    printf("%d\n", comp[j]);
                    j++; k++;
                }
                j--;
            }
            else if(comp[j]==5) {  // instrukcja wyboru
                ka=k;
                ja=j+1;
                bracket=1;
                while(bracket!=0) {  // przypadek zagniezdzenia
                    if(comp[ja]>=0 && comp[ja]<=9) {
                        if(comp[ja]==6) bracket--;
                        else if(comp[ja]==4 || comp[ja]==5) bracket++;
                        ka++;
                    }
                    ja++;
                }
                printf("5 %d\n", ka+1);
                k++;
            }
        }
        else if(comp[j]==6) {  // skok za instrukcje wyboru
            ka=k;
            ja=j+1;
            bracket=1;
            while(bracket!=0) {  // przypadek zagniezdzenia
                if(comp[ja]==8 || comp[ja]==800) bracket--;
                else if(comp[ja]==6) bracket++;
                if(comp[ja]>=0 && comp[ja]<=9) ka++;
                ja++;
            }
            printf("6 %d\n", ka+1);
            k++;
        }
        else if(comp[j]==8 && k!=line) {  // koniec instrukcji
            printf("8\n");
            k++;
        }
        else if(comp[j]==7) {  // skok do danej instrukcji
            j++; k++;
            printf("7 %d\n", stack[comp[j]*(-1)-265]);
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
