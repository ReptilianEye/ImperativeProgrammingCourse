//
// Created by piotr on 10.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 1 // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        scanf("%lf", x++);
}

void print_vec(const double x[], size_t n)
{
    for (size_t i = 0; i < n; ++i)
        printf("%.4f ", x[i]);
    printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n)
{
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
            scanf("%lf", &A[i][j]);
    }
}

void print_mat(const double A[][SIZE], size_t m, size_t n)
{
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
            printf("%.4f ", A[i][j]);
        printf("\n");
    }
    printf("\n");
}

void print_mat2(const double A[][SIZE], const int perm[], size_t n)
{
    int k;
    for (size_t i = 0; i < n; ++i)
    {
        for (k = 0; k < n; k++)
        {
            if (perm[k] == i)
                break;
        }
        for (size_t j = 0; j < n; ++j)
            printf("%.4f ", A[k][j]);
        printf("\n");
    }
    printf("\n");
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.
int change_with_highest(int row, int col, double A[][SIZE], int perm[], size_t n)
{
    double max_v = -1;
    int poz_max;
    double t;
    for (int i = row; i < n; i++)
    {
        t = fabs(A[perm[i]][col]);
        if (t > max_v)
        {
            max_v = t;
            poz_max = i;
        }
    }
    int temp = perm[row];
    perm[row] = perm[poz_max];
    perm[poz_max] = temp;
    return perm[row];
}

int to_diagonal(double A[][SIZE], int perm[], size_t n, double eps)
{
    int curr, j;
    print_mat(A, n, n);
    double multip;
    for (int i = 0; i < n; i++)
    {
        {
            curr = change_with_highest(i, i, A, perm, n);
            if (A[curr][i] < eps)
                return 0;
            j = i;
            while (j < n)
            {
                if (perm[j] != curr)
                {
                    multip = A[perm[j]][i] / A[curr][i];
                    for (int k = 0; k < n; k++)
                    {
                        A[perm[j]][k] = A[perm[j]][k] - A[curr][k] * multip;
                    }
                    //                    print_mat(A, n, n);
                }
                j++;
            }
        }
        //        print_mat(A, n, n);
    }
    return 1;
}

void change_cols_with_rows(double A[][SIZE], const int perm[], size_t n)
{
    int i, j, k;
    double temp;
    for (i = 0; i < n; i++)
    {
        for (k = 0; k < n; k++)
            if (perm[k] == i)
                break;
        for (j = i + 1; j < n; j++)
        {
            temp = A[j][k];
            A[j][k] = A[k][j];
            A[k][j] = temp;
        }
    }
}

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps)
{
    int perm[n];
    int res;
    double det;
    //    print_mat(A, n, n);
    for (int i = 0; i < n; i++)
    {
        perm[i] = i;
    }
    res = to_diagonal(A, perm, n, eps);
    print_mat2(A, perm, n);
    if (!res)
        return res;
    change_cols_with_rows(A, perm, n);
    print_mat2(A, perm, n);
    res = to_diagonal(A, perm, n, eps);
    print_mat2(A, perm, n);
    if (!res)
        return res;
    int j;
    for (size_t i = 0; i < n; ++i)
    {
        for (j = 0; j < n; j++)
        {
            if (perm[j] == i)
                break;
        }
        det = det * A[j][i];
    }
    return det;
}
// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps)
{
}

int main(void)
{

    setbuf(stdout, 0);

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m, n, p, q;
    if (TEST)
        printf("Wpisz nr zadania ");
    scanf("%d", &to_do);
    switch (to_do)
    {
    case 1:
        if (TEST)
            printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
        scanf("%d", &n);
        if (TEST)
            printf("Wpisz macierz A (wierszami): ");
        read_mat(A, n, n);
        if (TEST)
            printf("Wpisz wektor b: ");
        read_vec(b, n);
        det = gauss(A, b, x, n, eps);
        printf("%.4f\n", det);
        if (det)
            print_vec(x, n);
        break;
    case 2:
        if (TEST)
            printf("Wpisz rozmiar macierzy n = ");
        scanf("%d", &n);
        if (TEST)
            printf("Wpisz elementy macierzy (wierszami): ");
        read_mat(A, n, n);
        det = matrix_inv(A, B, n, eps);
        printf("%.4f\n", det);
        if (det)
            print_mat(B, n, n);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
