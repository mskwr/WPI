#include <stdio.h>
#ifndef N
#define N 5
#endif

void generate(int a[][N], int kolor) { // generowanie sciany wypelnionej danym kolorem
    int i=0;
    int j=0;
    while(i<N) {
        while(j<N) {
            a[j][i]=kolor;
            j++;
        }
        j=0;
        i++;
    }
}

void RubikCube(int a[][N], int b[][N], int c[][N], int d[][N], int e[][N], int f[][N]) {
    generate(a, 0);
    generate(b, 1);
    generate(c, 2);
    generate(d, 3);
    generate(e, 4);
    generate(f, 5);
    }

void BaseWalls(int a[][N], int i) { // wypisywanie podstaw kostki
    int space=0;
    int j=0;
    while(space<N+1) {
        printf(" ");
        space++;
    }
    while(j<N) {
        printf("%d", a[j][i]);
        j++;
    }
    printf("\n");
}

void SideWalls(int a[][N], int i) { // wypisywanie jednego wiersza scian bocznych kostki
    int j=0;
    while(j<N) {
        printf("%d", a[j][i]);
        j++;
    }
}

void print(int a[][N], int b[][N], int c[][N], int d[][N], int e[][N], int f[][N]) {
    int i=0;
    printf("\n");
    while(i<N) {
        BaseWalls(a, i);
        i++;
    }
    i=0;
    while(i<N) {
        SideWalls(b, i);
        printf("|");
        SideWalls(c, i);
        printf("|");
        SideWalls(d, i);
        printf("|");
        SideWalls(e, i);
        printf("\n");
        i++;
    }
    i=0;
    while(i<N) {
        BaseWalls(f, i);
        i++;
    }
}

int main(void)
{
    int UP[N][N], LEFT[N][N], FRONT[N][N], RIGHT[N][N], BACK[N][N], DOWN[N][N]; // sciany kostki
    RubikCube(UP, LEFT, FRONT, RIGHT, BACK, DOWN);
    return 0;
}
