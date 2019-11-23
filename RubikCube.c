#include <stdio.h>
#ifndef N
#define N 10
#endif

void generate(int a[][N][N]) {// funkcja zapelnia kostke podanymi liczbami-kolorami
    int i=0;
    int j=0;
    int k=0;
    for(k=0; k<6; k++) {
        for(i=0; i<N; i++) {
            for(j=0; j<N; j++) a[k][j][i]=k;
        }
    }
}

void printBaseWalls(int a[][N][N], int base) {  // funkcja wypisuje podstawe kostki
    int i=0;
    int j=0;
    int spaces=0;
    for(j=0; j<N; j++) {
        for(spaces=0; spaces<N+1; spaces++) printf(" ");
        for(i=0; i<N; i++) printf("%d", a[base][j][i]);
        printf("\n");
    }
}

void printSideWalls(int a[][N][N]) {  // funkcja wypisuje sciany boczne kostki
    int i=0;
    int j=0;
    int side=1;
    for(j=0; j<N; j++) {
        for(side=1; side<5; side++) {
            for(i=0; i<N; i++) printf("%d", a[side][j][i]);
            if(side<4) printf("|");
        }
        printf("\n");
    }
}

void print(int a[][N][N]) {  // funkcja wypisujaca cala kostke
    printf("\n");
    printBaseWalls(a, 0);
    printSideWalls(a);
    printBaseWalls(a, 5);
}

void transposition(int a[][N][N], int side) {  // funkcja robiąca transpozycje danej sciany
    int b[N][N]={0};
    int i=0;
    int j=0;
    int k=0;
    for(j=0; j<N; j++) {
        for(i=0; i<N; i++) b[j][i]=a[side][j][i];
    }
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) a[side][j][i]=b[N-1-i][j];
    }
}

void transposition2(int a[][N][N], int side) {  // funkcja robiaca transpozycje w przeciwna strone
    int b[N][N]={0};
    int i=0;
    int j=0;
    int k=0;
    for(j=0; j<N; j++) {
        for(i=0; i<N; i++) b[j][i]=a[side][j][i];
    }
    for(i=0; i<N; i++) {
        for(j=0; j<N; j++) a[side][j][i]=b[i][N-1-j];
    }
}

void rotation(int a[][N][N], int side, int layer) {  // funkcja rotujaca kolorami podczas obrotu warstw
    int i=0;
    int k=0;
    int b[N]={0};
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

void move(int a[][N][N], int side, int layer, int times) { // glowna funkcja ruchu kostki
    int i=0;
    if(layer==0) i=times;
    while(i<times) {
        transposition(a, side);
        rotation(a, side, layer);
        if(layer==N) {
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

void read(int a[][N][N]) {  // funkcja odczytujaca input
    int c=0;
    int side=0;
    int layer=0;
    int layervar=0;  // licznik sprawdzajacy czy zostala podana liczba warstw
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
            else layervar=-1;
            if(c!='.') {
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
                    while(c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' ||
                           c=='9') {
                        layer=(c-48)+layer*10;  // "-48" ze wzgledu na kod ASCI cyfr 0-9
                        c=getchar();
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
                move(a, side, layer, times);
                if(c=='\n') {
                    print(a);
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
