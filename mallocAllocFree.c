//
// Created by piotr on 17.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// funkcja, ktora zwraca zaalokowana dynamiczne (pamiec?)
int **allocate_matrix(int rows, int cols)
{

    int **A = (int **)malloc(rows * sizeof(int *));

    if (A == NULL)
        return NULL;

    for (int i = 0; i < rows; ++i)
    {

        A[i] = malloc(cols * sizeof(int));

        if (A[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(A[i]);
            free(A);
            return NULL;
        }
    }
    return A;
}

void free_matrix(int **A, int rows)
{
    for (int i = 0; i < rows; ++i)
        free(A[i]);
    free(A);
}

void generate_matrix(int **A, int rows, int cols, int start, int end)
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            A[i][j] = rand() % (end - start) + start;
}

void print_matrix(int **A, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
            printf("%4d, ", A[i][j]);
        printf("\n");
    }
}

void test_allocate_matrix()
{
    int rows = 6, cols = 10;
    int **A = allocate_matrix(rows, cols);
    if (A == NULL)
        return;
    generate_matrix(A, rows, cols, -10, 20);
    print_matrix(A, rows, cols);
    free_matrix(A, rows);
}

// funkcja, ktora zwalnia zaalokowana dynamiczne (pamiec?)

int main()
{
    test_allocate_matrix();
    return 0;
}