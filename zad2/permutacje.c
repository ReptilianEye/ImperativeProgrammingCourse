//
// Created by piotr on 13.03.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define N 100

#define TEST 1 // 1 - dla testowania
// 0 - dla oceny automatycznej

void swap(int *a, int *b)
{
    //    *a = *a + *b;
    //    *b = *a - *b;
    //    *a = *a - *b;
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Losuje jedna liczbe calkowita z przedzialu [a,b] stosujac funkcje rand() i operator %
// Jezeli a>b to zwraca INT_MIN
// Jezeli b-a > RAND_MAX to zwraca INT_MAX
// Jezeli a=b to zwraca a (bez wywolywania funkcji rand)
// Else zwraca liczbe z przedzialu [a,b]
int rand_from_interval(int a, int b)
{
    if (a > b)
        return INT_MIN;
    if (b - a > RAND_MAX)
        return INT_MAX;
    if (a == b)
        return a;
    return a + rand() % (b - a + 1);
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[])
{
    int i, rand_poz;
    for (i = 0; i < n; i++)
        tab[i] = i;
    for (i = 0; i < n - 1; i++)
    {
        rand_poz = rand_from_interval(i, n - 1);
        swap(&tab[i], &tab[rand_poz]);
    }
}

// Metoda babelkowa sortowania n elementow tablicy tab w porzadku od wartosci najmniejszej do najwiekszej.
// Zwraca numer iteracji petli zewnetrznej (liczony od 1), po ktorej tablica byla uporzadkowana,
// np. dla { 0 1 2 3 7 4 5 6 } -> 1,
//     dla { 1 2 3 7 4 5 6 0 } -> 7,
//     dla { 0 1 2 3 4 5 6 7 S} -> 0.
int bubble_sort(int n, int tab[])
{
    int iters, i, if_done;
    iters = 0;
    if_done = 1;
    while (1)
    {
        for (i = 0; i < n - 1; i++)
            if (tab[i] > tab[i + 1])
            {
                if_done = 0;
                swap(&tab[i], &tab[i + 1]);
            }
        if (if_done)
            break;
        else
            if_done = 1;
        iters += 1;
    }
    //    for (i = 0; i < n; i++)
    //        printf("%d, ", tab[i]);
    return iters;
}

int main(void)
{
    int nr_testu, seed;
    int a, b, n, i;
    int tab[N];
    if (TEST)
        printf("Wpisz nr testu i seed = ");
    scanf("%d %d", &nr_testu, &seed);
    srand(seed); // ustawienie ziarna generatora (dla powterzalnosci wynikow)
    switch (nr_testu)
    {
    case 1:
        if (TEST)
            printf("a b = ");
        scanf("%d %d", &a, &b);
        for (int i = 0; i < 3; ++i)
            printf("%d ", rand_from_interval(a, b));
        break;
    case 2:
        if (TEST)
            printf("n = ");
        scanf("%d", &n);
        rand_permutation(n, tab);
        for (i = 0; i < n; ++i)
            printf("%d ", tab[i]);
        printf("\n");
        break;
    case 3:
        if (TEST)
            printf("n = ");
        scanf("%d", &n);
        rand_permutation(n, tab);
        printf("%d\n", bubble_sort(n, tab));
        break;
    default:
        if (TEST)
            printf("NOTHING TO DO!\n");
    }
    return 0;
}
