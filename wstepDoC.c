#include <stdio.h>
#include <float.h>

int main(void) {
    double tab[] = {-00.1, 2, 4, 5.1, 4e1};
    double max = DBL_MIN;
    int osemkowa = 012; // jesli damy 0 przed liczbą to ta liczba jest przekliczana na system osemkowy
    int szesnastowa = 0xA; // jesli damy 0x przed liczbą to ta liczba jest w systemie szesnastkowym
    char a[] = "abc\b\bCDE";
    int b = '1';
    int c, d;
    c = 1;
    d = 5;
    int temp = c++ + d; //(c++) +d
    printf("%d\n", b);

    int suma = 0;
    loop:
    suma = suma + 1;
    if (suma < 20)
        goto loop;

    printf("%f", max);
    return 0;
}