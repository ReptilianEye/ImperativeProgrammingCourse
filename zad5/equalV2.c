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

void print_vec2(const double x[], const int perm[], size_t n)
{
    int k;
    for (size_t i = 0; i < n; ++i)
    {
        for (k = 0; k < n; k++)
        {
            if (perm[k] == i)
                break;
        }
        printf("%.4f ", x[k]);
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
}

void print_data(const double A[][SIZE], const double x[], const int perm[], size_t n)
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
        printf("%.4f \n", x[k]);
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
    for (int i = 0; i < n; i++)
    {
        if (perm[i] >= col)
        {
            t = A[i][col];
            t = fabs(t);
            if (t > max_v)
            {
                max_v = t;
                poz_max = i;
            }
        }
    }
    int temp = perm[row];
    perm[row] = perm[poz_max];
    perm[poz_max] = temp;
    return perm[row];
}

void update_down(double A[][SIZE], int perm[], double x[], size_t n, int it, int prow)
{
    double multip;
    for (int i = 0; i < n; i++)
    {
        if (perm[i] > it)
        {
            multip = A[i][it] / A[prow][it];
            for (int k = 0; k < n; k++)
            {
                A[i][k] = A[i][k] - A[prow][k] * multip;
            }
            x[i] = x[i] - x[prow] * multip;
            //            print_data(A, x, perm, n);
        }
    }
}

double to_diagonal_down(double A[][SIZE], int perm[], double x[], size_t n, double eps)
{
    int curr, row;
    for (int i = 0; i < n - 1; i++)
    {
        {
            for (row = 0; row < n; row++)
                if (perm[row] == i)
                    break;
            curr = change_with_highest(row, i, A, perm, n);
            if (A[curr][i] < eps)
                return 0;
            //            print_data(A, x, perm, n);
            update_down(A, perm, x, n, i, curr);
        }
    }
    return 1;
}

void update_up(double A[][SIZE], int perm[], double x[], size_t n, int it, int prow)
{

    double multip;
    for (int i = 0; i < n; i++)
    {
        if (perm[i] < it)
        {
            multip = A[i][it] / A[prow][it];
            for (int k = 0; k < n; k++)
            {
                A[i][k] = A[i][k] - A[prow][k] * multip;
            }
            x[i] = x[i] - x[prow] * multip;
            //            print_data(A, x, perm, n);
        }
    }
}

int to_diagonal_up(double A[][SIZE], int perm[], double x[], size_t n, double eps)
{
    int curr;
    for (int i = (int)n - 1; i > 0; i--)
    {
        {
            for (curr = 0; curr < n; curr++)
            {
                if (perm[curr] == i)
                    break;
            }
            update_up(A, perm, x, n, i, curr);
            //            print_data(A, x, perm, n);
        }
    }
    return 1;
}

double calc_det(const double A[][SIZE], const int perm[], size_t n)
{
    double det = 1;
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

void tidy_diag(double A[][SIZE], double x[], size_t n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            if (A[i][j] != 0)
            {
                x[i] = x[i] / A[i][j];
                A[i][j] = 1;
            }
    }
}

int reverse_perms(double x[], int perm[], size_t n)
{
    int times = 0;
    int looking_for, i, j;
    double temp;
    for (i = 0; i < n; i++)
    {
        if (perm[i] != i)
        {
            looking_for = perm[perm[i]];
            temp = x[i];
            x[i] = x[perm[i]];
            x[perm[i]] = temp;
            perm[i] = looking_for;
            times++;
            //            print_vec(x, n);
        }
    }
    return times;
}

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps)
{
    int perm[n];
    for (int i = 0; i < n; i++)
    {
        x[i] = b[i];
        perm[i] = i;
    }
    double det;
    //    print_data(A, x, perm, n);

    det = to_diagonal_down(A, perm, x, n, eps);
    if (!det)
        return 0;

    to_diagonal_up(A, perm, x, n, eps);
    det = calc_det(A, perm, n);
    tidy_diag(A, x, n);
    //    print_vec(x, n);
    int times_reversed = reverse_perms(x, perm, n);
    det = times_reversed % 2 == 0 ? fabs(det) : -fabs(det);
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
