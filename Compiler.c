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

int main(void)
{
    int cell=0;
    return 0;
}
