/**
 * Zadanie zaliczeniowe 1 - kostka Rubika
 * Michał Skwarek
 * ms418426
 * gr. 4 - MD
 */

#include <stdio.h>
#ifndef N
#define N 5
#endif

/**
 * Funkcja generująca kostkę Rubika
 * Wypełnia tablice kolejnymi liczbami zgodnymi z treścią
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 */

void generate(int a[][N][N]) {
    int i=0;
    int j=0;
    int k=0; // liczba, które wypełnia tablicę
    for(k=0; k<6; k++) {
        for(i=0; i<N; i++) {
            for(j=0; j<N; j++) a[k][j][i]=k;
        }
    }
}

/**
 * Funkcja wypisująca liczby jednej ściany w formacie,
 * w jakim powinny być wypisane podstawy kostki Rubika - ściany u i d
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 *  base - numer ściany, którą chcemy w ten sposób wypisać
 */

void printBaseWalls(int a[][N][N], int base) {
    int i=0;
    int j=0;
    int spaces=0; // liczba spacji przed wypisywaniem liczb/kolorów
    for(j=0; j<N; j++) {
        for(spaces=0; spaces<N+1; spaces++) printf(" ");
        for(i=0; i<N; i++) printf("%d", a[base][j][i]);
        printf("\n");
    }
}

/**
 * Funkcja wypisująca liczby wszystkich ścian bocznych w formacie,
 * w jakim powinny być wypisane ściany boczne kostki Rubika - l, f, r i b
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 */

void printSideWalls(int a[][N][N]) {
    int i=0;
    int j=0;
    int side=1; // numer wypisywanej ściany
    for(j=0; j<N; j++) {
        for(side=1; side<5; side++) {
            for(i=0; i<N; i++) printf("%d", a[side][j][i]);
            if(side<4) printf("|");
        }
        printf("\n");
    }
}

/**
 * Funkcja wypisująca całą kostkę w określonym formacie
 * Złożenie funkcji wypisujących podstawy i ściany boczne
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 */

void print(int a[][N][N]) {
    printf("\n");
    printBaseWalls(a, 0);
    printSideWalls(a);
    printBaseWalls(a, 5);
}

/**
 * Funkcja zmieniająca liczby/kolory na ścianie przy jej obrocie
 * o 90 stopni zgodnie z ruchem wskazówek zegara
 *
 * Argumenty: 
 *  a - trójwymiarowa tablica - kostka Rubika
 *  side - numer ściany, która ma się obrócić
 */

void transposition(int a[][N][N], int side) {
    int b[N][N]={0};
    int i=0;
    int j=0;
    for(j=0; j<N; j++) {
        for(i=0; i<N; i++) b[j][i]=a[side][j][i];
    }
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) a[side][j][i]=b[N-1-i][j];
    }
}

/**
 * Funkcja zmieniająca liczby/kolory na ścianie przy jej obrocie
 * o 90 stopni przeciwnie do ruchu wskazówek zegara
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 *  side - numer ściany, która ma się obrócić
 */

void transposition2(int a[][N][N], int side) {
    int b[N][N]={0};
    int i=0;
    int j=0;
    for(j=0; j<N; j++) {
        for(i=0; i<N; i++) b[j][i]=a[side][j][i];
    }
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) a[side][j][i]=b[i][N-1-j];
    }
}

/**
 * Funkcja zmieniające liczby/kolory na ścianach w wyniku obrotu warstw
 * W przypadku obrotu każdej ściany odpowiednio zmienia indeksy
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 *  side - numer ściany, która ma się obrócić
 *  layer - liczba warstw, które mają się obrócić
 */

void rotation(int a[][N][N], int side, int layer) {
    int i=0;
    int k=0;
    int b[N]={0}; // tablica pomocnicza
    for(k=0; k<layer; k++) {
        if(side==0) {
            for(i=0; i<N; i++) b[i]=a[1][k][i];
            for(i=0; i<N; i++) a[1][k][i]=a[2][k][i];
            for(i=0; i<N; i++) a[2][k][i]=a[3][k][i];
            for(i=0; i<N; i++) a[3][k][i]=a[4][k][i];
            for(i=0; i<N; i++) a[4][k][i]=b[i];
        }
        else if(side==1) {
            for(i=0; i<N; i++) b[i]=a[0][i][k];
            for(i=0; i<N; i++) a[0][i][k]=a[4][N-1-i][N-1-k];
            for(i=0; i<N; i++) a[4][N-1-i][N-1-k]=a[5][i][k];
            for(i=0; i<N; i++) a[5][i][k]=a[2][i][k];
            for(i=0; i<N; i++) a[2][i][k]=b[i];
        }
        else if(side==2) {
            for(i=0; i<N; i++) b[i]=a[0][N-1-k][i];
            for(i=0; i<N; i++) a[0][N-1-k][i]=a[1][N-1-i][N-1-k];
            for(i=0; i<N; i++) a[1][N-1-i][N-1-k]=a[5][k][N-1-i];
            for(i=0; i<N; i++) a[5][k][N-1-i]=a[3][i][k];
            for(i=0; i<N; i++) a[3][i][k]=b[i];
        }
        else if(side==3) {
            for(i=0; i<N; i++) b[i]=a[0][i][N-1-k];
            for(i=0; i<N; i++) a[0][i][N-1-k]=a[2][i][N-1-k];
            for(i=0; i<N; i++) a[2][i][N-1-k]=a[5][i][N-1-k];
            for(i=0; i<N; i++) a[5][i][N-1-k]=a[4][N-1-i][k];
            for(i=0; i<N; i++) a[4][N-1-i][k]=b[i];
        }
        else if(side==4) {
            for(i=0; i<N; i++) b[i]=a[0][k][i];
            for(i=0; i<N; i++) a[0][k][i]=a[3][i][N-1-k];
            for(i=0; i<N; i++) a[3][i][N-1-k]=a[5][N-1-k][N-1-i];
            for(i=0; i<N; i++) a[5][N-1-k][N-1-i]=a[1][N-1-i][k];
            for(i=0; i<N; i++) a[1][N-1-i][k]=b[i];
        }
        else {
            for(i=0; i<N; i++) b[i]=a[2][N-1-k][i];
            for(i=0; i<N; i++) a[2][N-1-k][i]=a[1][N-1-k][i];
            for(i=0; i<N; i++) a[1][N-1-k][i]=a[4][N-1-k][i];
            for(i=0; i<N; i++) a[4][N-1-k][i]=a[3][N-1-k][i];
            for(i=0; i<N; i++) a[3][N-1-k][i]=b[i];
        }
    }
}

/**
 * Główna funkcja ruszająca kostką (zmieniająca kolory/liczby)
 * podczas obrotu danej liczby warstw ściany daną liczbę razy
 * 
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 *  side - numer ściany, która ma się obracać
 *  layer - ilość warstw ściany, która ma się obrócić
 *  times - wielokrotność 90 stopni o jaką ściana ma się obrócić
 */

void move(int a[][N][N], int side, int layer, int times) {
    int i=0;
    if(layer==0) i=times;
    while(i<times) {
        transposition(a, side);
        rotation(a, side, layer);
        if(layer==N) { // przypadek dla obrotu wszystkich warstw
            if(side==0) transposition2(a, 5);
            else if(side==1) transposition2(a, 3);
            else if(side==2) transposition2(a, 4);
            else if(side==3) transposition2(a, 1);
            else if(side==4) transposition2(a, 2);
            else transposition2(a, 0);
        }
        i++;
    }
}

/**
 * Główna funkcja odczytująca znaki z klawiatury i przekazująca odpowiednie
 * argumenty side, layer oraz times do funkcji move poruszającej kostką
 *
 * Argumenty:
 *  a - trójwymiarowa tablica - kostka Rubika
 */

void read(int a[][N][N]) {
    int c=0; // wczytywany znak z klawiatury
    int side=0;
    int layer=0;
    int layervar=0;  // licznik sprawdzajacy czy użytkowni podał liczbe warstw
    int times=1;
    while(c!='.') {
        while(c!='.') {
            if(c=='\n') print(a);
            if(c=='u') side=0;
            else if(c=='l') side=1;
            else if(c=='f') side=2;
            else if(c=='r') side=3;
            else if(c=='b') side=4;
            else if(c=='d') side=5;
            else layervar=-1; // "-1" aby odróżnić czy użytkownik nie podał
            if(c!='.') {      // żadnej warstwy czy liczbę warstw "0"
                c=getchar();
                if(c=='\'') {
                    times=3;
                    c=getchar();
                }
                else if(c=='"') {
                    times=2;
                    c=getchar();
                }
                else {
                    while(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || 
                          c=='5' || c=='6' || c=='7' || c=='8' || c=='9') {
                        layer=(c-48)+layer*10;  // "-48" ze wzgledu na kod
                        c=getchar();            // ASCII cyfr 0-9
                        layervar++;
                    }
                }
                if(c=='\'') {
                    times=3;
                    c=getchar();
                }
                else if(c=='\"') {
                    times=2;
                    c=getchar();
                }
                if(layervar==0) layer=1;
                move(a, side, layer, times); // główna funkcja ruszająca kostką
                if(c=='\n') {
                    print(a); // główna funkcja wypisująca kostkę
                    c=getchar();
                }
                layervar=layer=side=0;
                times=1;
            }
        }
    }
}

int main(void)
{
    int RubikCube[6][N][N];
    generate(RubikCube);
    read(RubikCube);
    return 0;
}
