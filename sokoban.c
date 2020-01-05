/**
 * Zadanie zaliczeniowe 3 - sokoban
 * Michał Skwarek
 * ms418426
 * gr. 4 - MD
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Stworzenie nowej struktury: pola planszy
 *
 * Składniki struktury:
 * x - współrzędna x danego pola - numer kolumny planszy
 * y - współrzędna y danego pola - numer wiersza planszy
 * c - znak, który jest na danym polu
 * n - unikalny numer danego pola (kolejno od 0)
 */

typedef struct box {
    int x;
    int y;
    int c;
    int n;
} box;

/**
 * Stworzenie nowej struktury: kolejki
 *
 * Składniki struktury:
 * *A - tablica przechowująca wartości w kolejce
 * beg - początek kolejki
 * end - koniec kolejki
 */

typedef struct queue {
    int *A;
    int beg;
    int end;
} queue;

/**
 * Procedura umieszczenia danej wartości w kolejce
 *
 * Argumenty:
 * *Q - kolejka, do której chcemy umieścić wartość
 * x - wartość, którą chcemy umieścić w kolejce
 */

void put(queue *Q, int x) {
    (*Q).A[(*Q).end]=x;
    ((*Q).end)++;
}

/**
 * Funkcja pobierająca wartość i usuwająca ją z kolejki
 *
 * Argumenty:
 * *Q - kolejka, z której chcemy pobrać wartość
 *
 * Wynik:
 * x - wartość na początku kolejki
 */

int get(queue *Q) {
    int x;
    x=(*Q).A[(*Q).beg];
    ((*Q).beg)++;
    return x;
}

/**
 * Funkcja czytająca i zapisująca planszę do gry z pliku wejściowego
 *
 * Argumenty:
 * *size - ilość znaków w planszy
 *
 * Wynik:
 * board - tablica pól typu box zawierająca wszystkie pola planszy do gry
 */

box *read(int *size) {
    box *board=NULL;
    int i=0;
    int j=0;
    int c=0;
    int k=0;
    int var=0; // zmienna sprawdzająca gdzie w danych wejściowych są instrukcje
    int count=0;
    for(i=0; !var; i++) {
        c=getchar();
        if(i==*size) {
            count=0;
            *size=1+*size*2;
            board=realloc(board, *size*sizeof *board);
        }
        if(c=='\n') {
            j++;
            i--;
            k=0;
            count++;
            if(count==2) var=1;
        }
        else {
            count=0;
            board[i].x=k;
            board[i].y=j;
            board[i].c=c;
            board[i].n=i;
            k++;
        }
    }
    *size=i;
    return board;
}

/**
 * Procedura wypisująca planszę - kolejno pola z tablicy pól
 *
 * Argumenty:
 * board - tablica pól typu box zawierająca informacje o polach tablicy
 * size - ilość znaków w planszy
 */

void write(box *board, int size) {
    int k=0;
    for(int i=0; i<size; i++) {
        if(board[i].y>k) {
            printf("\n");
            k++;
        }
        printf("%c", board[i].c);
    }
    printf("\n");
}

/**
 * Procedura sprawdzająca czy istniejące pole tworzy ścieżkę do celu,
 * jeśli tak - wpisująca numer tego pola do kolejki
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * seen - tablica zawierająca informację o każdym z polu, czy było już odwiedzone
 * *s - numer pola, które chcemy sprawdzić
 * aim - pole docelowe, cel
 * *wait - kolejka, do której potencjalnie wpisujemy numery pól
 */

void check(box *board, int *seen, int *s, box aim, queue *wait) {
    if(board[*s].c=='+' || board[*s].c=='-' || board[*s].c=='@' || board[*s].c=='*') {
        if(board[*s].n==aim.n) *s=-1;
        else if(seen[*s]==0) {
            put(wait, *s);
            seen[*s]=-1;
        }
    }
    else {
        if(board[*s].n==aim.n) *s=-2;
        seen[*s]=-1;
    }
}

/**
 * Funkcja wyszukująca drogę do określonego pola i określająca, czy można do niego
 * dojść za pomocą algorytmu przechodzenia grafu wszerz (breadth-first search)
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * start - pole startowe, od którego szukamy możliwej drogi
 * aim - pole docelowe, do którego szukamy możliwą drogę
 * *wait - kolejka, do której wpisujemy pola możliwe do przejścia
 * *seen - tablica informująca czy pole o danym numerze było odwiedzone
 * *N - tablica zawierająca informację, ile jest znaków w danym wierszu
 * size - liczba znaków w planszy
 *
 * Wynik:
 * 0 - gdy do pola nie można legalnie dojść
 * 1 - gdy do pola można legalnie dojść
 * jeśli żadne z powyższych - rekurencyjnie dalsze szukanie drogi
 */

int BFS(box *board, box start, box aim, queue *wait, int *seen, int *N, int size) {
    int s=0;
    if(start.n==aim.n) return 1; // gdy cel jest znaleziony
    else if(seen[start.n]==1) {
        if((*wait).beg==(*wait).end) return 0; // gdy kolejka się skończyła
        else return BFS(board, board[get(wait)], aim, wait, seen, N, size);
    }
    else {
        seen[start.n]=1;
        if(start.x!=0) {
            s=start.n-1; // sprawdzenie lewego sąsiada (jeśli istnieje)
            check(board, seen, &s, aim, wait);
            if(s==-1) return 1;
            else if(s==-2) return 0;
        }
        if(start.n<size-1 && board[start.n+1].y==start.y) {
            s=start.n+1; // sprawdzenie prawego sąsiada (jeśli istnieje)
            check(board, seen, &s, aim, wait);
            if(s==-1) return 1;
            else if(s==-2) return 0;
        }
        if(start.y!=0 && board[start.n-N[start.y-1]].x==start.x) {
            s=start.n-N[start.y-1]; // sprawdzenie górnego sąsiada (jeśli istnieje)
            check(board, seen, &s, aim, wait);
            if(s==-1) return 1;
            else if(s==-2) return 0;
        }
        if(start.n+N[start.y]<size && board[start.n+N[start.y]].x==start.x) {
            s=start.n+N[start.y]; // sprawdzenie dolnego sąsiada (jeśli istnieje)
            check(board, seen, &s, aim, wait);
            if(s==-1) return 1;
            else if(s==-2) return 0;
        }
        if((*wait).beg==(*wait).end) return 0; // gdy kolejka się skończyła
        else return BFS(board, board[get(wait)], aim, wait, seen, N, size);
    }
}

/**
 * Funkcja określająca czy do danego pola można dojść, czy nie
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * size - ilość znaków w planszy
 * start - pole od którego chcemy znaleźć drogę
 * aim - pole, do którego chcemy dojść
 * N - tablica zawierająca informację, ile jest znaków w danym wierszu
 *
 * Wynik:
 * true - gdy istnieje legalna droga z pola start do pola aim
 * false - gdy nie istnieje legalna droga z pola start do pola aim
 */

bool reachable(box *board, int size, box start, box aim, int *N) {
    queue *wait=malloc(sizeof(queue));
    (*wait).A=(int*)malloc(size*sizeof(int));
    int seen[size];
    int j=0;
    for(j=0; j<size; j++) {
        seen[j]=0;
        (*wait).A[j]=0;
    }
    (*wait).beg=0;
    (*wait).end=0;
    if(aim.c!='+' && aim.c!='-' && aim.c!='@' && aim.c!='*') {
        free((*wait).A);
        free(wait);
        return false; // gdy na szukane pole postać nie może legalnie wejść
    }
    if(BFS(board, start, aim, wait, seen, N, size)) {
        free((*wait).A);
        free(wait);
        return true; // gdy droga do pola została znaleziona funkcją BFS
    }
    else {
        free((*wait).A);
        free(wait);
        return false; // gdy droga nie została znaleziona przez funkcję BFS
    }
}

/**
 * Funkcja szukająca na planszy danych znaków
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * size - ilość znaków z których składa się plansza
 * sign1 - pierwszy wyszukiwany znak
 * sign2 - drugi wyszukiwany znak
 *
 * Wynik:
 * n - numer pola, na którym znajduje się dany znak
 */

int search(box *board, int size, int sign1, int sign2) {
    int n=-1;
    int i=0;
    for(i=0; i<size && board[i].c!=sign1 && board[i].c!=sign2; i++);
    if(i<size) n=i; // gdy znak nie został znaleziony
    return n;
}

/**
 * Funkcja zmieniająca znaki na planszy podczas przesunięcia skrzyni
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * chest - znak skrzyni, którą postać chce przesunąć
 * aim - numer pola na którym jest skrzynia, którą postać chce przesunąć
 * me - numer pola, na którym znajduje się postać
 * x - numer pola, na które postać chce przesunąć skrzynię
 * p - znak skrzyni przed ostatnim przesunięciem
 *
 * Wynik:
 * board - zmodyfikowana plansza sokobanu
 */

box *tangle(box *board, int chest, int aim, int me, int x, int p) {
    int m=-1;
    int ch=-1;
    if(board[me].c=='*') m='+';
    else if(board[me].c=='@') m='-';
    if(board[aim].c==chest) ch='@';
    else if(board[aim].c==chest-32) ch='*';
    board[me].c=m;
    board[aim].c=ch;
    board[x].c=p;
    return board;
}

/**
 * Funkcja określająca w jaki sposób powinny zostać zmienione znaki przy pchnięciu
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * chest - znak skrzyni, którą postać chce przesunąć
 * aim - numer pola na którym jest skrzynia do przesunięcia
 * me - numer pola, na którym znajduje się postać
 * x - numer pola, na które postać chce przesunąć skrzynię
 *
 * Wynik:
 * board - zmodyfikowana plansza sokobanu
 */

box *change(box *board, int chest, int aim, int me, int x) {
    int p=0;
    if(board[x].c=='+' || board[x].c=='*') {
        p=chest-32; // w przypadku pola 'docelowego'
        return tangle(board, chest, aim, me, x, p);
    }
    else {
        p=chest; // gdy pole nie jest polem 'docelowym'
        return tangle(board, chest, aim, me, x, p);
    }
}

/**
 * Funkcja poruszająca kostką i zapisująca poprzednie współrzędne skrzyni i postaci
 *
 * Argumenty:
 * *board - tablica zawierająca wszystkie pola planszy sokobanu
 * *back - tablica pól zawierająca informacje o poprzednich współrzędnych
 * size - ilość znaków w planszy sokobanu
 * chest - znak skrzyni, którą postać chce pchnąć
 * *N - tablica zawierająca informacje o ilościach znaków w kolejnych wierszach
 * **j - ilość pchnięć skrzyń, które można cofnąć
 * me - numer pola, na którym znajduje się postać
 * aim - numer pola, na którym znajduje się skrzynia
 * t - numer pola, na któ©e ma być pchnięta skrzynia
 * r - numer pola, na które ma dotrzeć postać, by móc pchnąć skrzynię
 *
 * Wynik:
 * board - plansza po dokonaniu pchnięcia skrzyni przez postać
 */

box *move(box *board, box *back, int size, int chest, int *N, int **j, int me, int aim, int t, int r) {
    if(reachable(board, size, board[me], board[r], N)) {
        if(board[t].c=='+' || board[t].c=='-' || board[t].c=='@' || board[t].c=='*') {
            back[**j].n=me;
            back[**j].x=aim;
            back[**j].y=t;
            (**j)++;
            return change(board, chest, aim, me, t);
        }
        else return board; // gdy pole, na które ma być pchnięta skrzynia jest zablokowane
    }
    else return board;
}

/**
 * Funkcja przetwarzająca instrukcję z danych wejściowych na zmianę pól planszy
 *
 * Argumenty:
 * *board - plansza pól do gry w sokobana
 * size - ilość znaków w planszy sokobanu
 * chest - znak skrzyni, która ma być pchnięta
 * dir - znak kierunku, w którym ma być pchnięta skrzynia
 * *N - ilość znaków w danym wierszu planszy
 * *j - ilość możliwych do cofnięcia pchnięć skrzyni przez postać
 * *back - tablica współrzędnych pól, do których można cofnąć grę
 *
 * Wynik:
 * board - plansza po wykonaniu pchnięcia skrzyni
 */

box *compute(box *board, int size, int chest, int dir, int *N, int *j, box *back) {
    int t=0; // numer pola, na które postać chce pchnąć skrzynię
    int p=0; // numer pola, na które postać musi dojść, gdy pchnąć skrzynię
    int aim=search(board, size, chest, chest-32);
    int me=search(board, size, '@', '*');
    if(aim==-1 || me==-1) return board; // gdy nie ma pola z postacią
    if(dir==8 || dir==2) { // przesunięcie skrzyni w górę lub w dół
        t=aim+N[board[aim].y];
        p=aim-N[board[aim].y-1];
        if(t<size && board[aim].y!=0 && board[p].x==board[aim].x && board[aim].x==board[t].x) {
            if(dir==8) return move(board, back, size, chest, N, &j, me, aim, p, t);
            else return move(board, back, size, chest, N, &j, me, aim, t, p);
        }
        else return board;
    }
    else { // przesunięcie skrzyni w lewo lub w prawo
        t=aim+1;
        p=aim-1;
        if(board[aim].x!=0 && aim<size-1 && board[t].y==board[aim].y) {
            if(dir==4) return move(board, back, size, chest, N, &j, me, aim, p, t);
            else return move(board, back, size, chest, N, &j, me, aim, t, p);
        }
        else return board;
    }
}

/**
 * Funkcja cofająca ostatnie pchnięcie skrzyni
 *
 * Argumenty:
 * *board - plansza do gry w sokobana
 * *back - tablica zawierająca współrzędne możliwych do cofnięcia pchnięć
 * *j - ilość pchnięć skrzyni, które można cofnąć
 *
 * Wynik:
 * board - zmodyfikowana plansza po cofnięciu ostatniego pchnięcia
 */

box *rew(box *board, box *back, int *j) {
    char chest1=0;
    char chest2=0;
    char me=0;
    (*j)--;
    if(*j==-1) { // gdy nie ma żadnych pchnięć, które można cofnąć
        *j=0;
        return board;
    }
    else {
        if(board[back[*j].n].c=='+' || (board[back[*j].n].c>='A' && board[back[*j].n].c<='Z')) {
            me='*';
        }
        else if(board[back[*j].n].c=='-' || (board[back[*j].n].c>='a' && board[back[*j].n].c<='z')) {
            me='@';
        }
        if(board[back[*j].y].c>='a' && board[back[*j].y].c<='z') chest2='-';
        else if(board[back[*j].y].c>='A') chest2='+';
        chest1=(char)(board[back[*j].y].c);
        if(board[back[*j].x].c=='@') {
            if(chest1>='A' && chest1<='Z') chest1=(char)((int)(chest1)+32);
        }
        else if(board[back[*j].x].c=='*') {
            if(chest1>='a' && chest1<='z') chest1=(char)((int)(chest1)-32);
        }
        board[back[*j].y].c=chest2;
        board[back[*j].n].c=me;
        board[back[*j].x].c=chest1;
        return board;
    }
}

/**
 * Procedura pobierająca instrukcje z danych wejściowych i przetwarzająca je na
 * odpowiednie modyfikacje planszy - pchnięcia skrzyń lub cofanie pchnięć
 */

void play() {
    int size=0;
    box *board=read(&size);
    box *back=NULL;
    write(board, size);
    int c=0;
    int cell=0;
    int chest=0; // znak skrzyni do przesunięcia
    int dir=0; // kierunek w którym ma być przesunięta skrzynia
    int j=0;
    int k=0;
    int sum=0;
    int N[size];
    for(int i=0; i<size; i++) {
        if(board[i].y>board[k].y) {
            N[j]=board[i].n-board[k].n;
            k=i;
            sum+=N[j];
            j++;
        }
    }
    N[j]=size-sum;
    j=0;
    while((c=getchar())!='.') {
        while(c!='\n' && c!='.') {
            if(c=='0') {
                write(rew(board, back, &j), size);
                c=getchar();
            }
            else if(c>='a' && c<='z') {
                if(j>=cell) {
                    cell=1+cell*2;
                    back=realloc(back, cell*sizeof *back);
                }
                chest=c;
                c=getchar();
                dir=c-48;
                board=compute(board, size, chest, dir, N, &j, back);
                write(board, size);
                c=getchar();
            }
        }
    }
    free(back);
    free(board);
}

int main() {
    play();
    return 0;
}
