//
// Created by piotr on 15.05.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

// ex 1
int get(int cols, int row, int col, const int *A) {
    return A[row * cols + col];
}

void set(int cols, int row, int col, int *A, int value) {
    A[row * cols + col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB) {
    for (int col_B = 0; col_B < colsB; col_B++)
        for (int row_A = 0; row_A < rowsA; row_A++) {
            int cell_v = 0;
            for (int fast_A = 0, fast_B = 0; fast_A < colsA; fast_A++, fast_B++) {
                cell_v += get(colsA, row_A, fast_A, A) * get(colsB, fast_B, col_B, B);
            }
            set(colsB, row_A, col_B, AB, cell_v);
        }

}

void read_mat(int rows, int cols, int *t) {
    int val;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            scanf("%d", &val);
            set(cols, i, j, t, val);
        }

}

void print_mat(int rows, int cols, int *t) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", get(cols, i, j, t));
        }
        printf("\n");
    }
}

// ex 1 end
int read_char_lines(char *array[]) {

}

void write_char_line(char *array[], int n) {
    printf("%s", array[n]);
}

void delete_lines(char *array[]) {
    for (int i = 0; array[i] != NULL; i++)
        free(array[i]);
}
//ex 2

int read_int_lines_cont(int *ptr_array[]) {
    char line[BUF_SIZE], *p, *e;
    int v, lines_n = 0;
    while (fgets(line, sizeof(line), stdin)) {
        int *ptr = (int *) malloc(sizeof(int));
        ptr_array[lines_n] = ptr;
        lines_n++;
        p = line;
        for (p = line;; p = e) {
            v = strtol(p, &e, 10);
            if (p == e)
                break;
            *ptr = v;
            ptr++;
        }

    }
    return lines_n;
}

void write_int_line_cont(int *ptr_array[], int n) {
    int *ptr = ptr_array[n];
    while (*ptr != NULL) // or int n_elem = sizeof(ptr_array[n])/sizeof(*ptr_array[n])
    {
        printf("%d ", *ptr);
        *ptr = *(ptr + 1);
    }
}
//ex 2 end

typedef struct {
    int *values;
    int len;
    double average;
} line_type;

//ex 3
int read_int_lines(line_type lines_array[]) {
}

void write_int_line(line_type lines_array[], int n) {
}

//ex 3 end
void delete_int_lines(line_type array[], int line_count) {
}

int cmp(const void *a, const void *b) {
}

void sort_by_average(line_type lines_array[], int line_count) {
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets) {
    int r, c, v;
    for (int i = 0; i < n_triplets; i++) {
        scanf("%d %d %d", &r, &c, &v);
        triplet new = {r, c, v};
        triplet_array[i] = new;
    }
    return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2) {
    triplet *a = (triplet *) t1;
    triplet *b = (triplet *) t2;
    int row_d = a->r - b->r;
    if (row_d != 0)
        return row_d;
    return a->c - b->c;
}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
    for (int i = 0; i < n_triplets; i++) {
        triplet curr = triplet_array[i];
        V[i] = curr.v;
        C[i] = curr.c;
        R[curr.r + 1]++;
    }
    for (int i = 1; i < n_triplets; i++) {
        R[i] = R[i] + R[i - 1];
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y) {
    for (int i = 0; i < rows; i++) {
        y[i] = 0;
        int t1 = R[i];
        int t2 = R[i + 1];
        for (int j = R[i]; j < R[i + 1]; j++)
            y[i] = y[i] + V[j] * x[C[j]];
    }
}

void read_vector(int *v, int n) {
    for (int i = 0; i < n; i++)
        scanf("%d", &v[i]);
}

void write_vector(int *v, int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", v[i]);
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int) strtol(c_buf, NULL, 10);
}

int main(void) {
    setbuf(stdout, 0);
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = {NULL};
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do) {
        case 1:
            scanf("%d %d", &rowsA, &colsA);
            read_mat(rowsA, colsA, A);
//            print_mat(rowsA, colsA, A);
            scanf("%d %d", &rowsB, &colsB);
            read_mat(rowsB, colsB, B);
//            print_mat(rowsB, colsB, B);
            prod_mat(rowsA, colsA, colsB, A, B, AB);
            print_mat(rowsA, colsB, AB);
            break;
        case 2:
            n = read_int() - 1; // we count from 1 :)
            ptr_array[0] = continuous_array;
            read_int_lines_cont(ptr_array);
            write_int_line_cont(ptr_array, n);

            break;
        case 3:
            n = read_int() - 1;
            read_char_lines(char_lines_array);
            write_char_line(char_lines_array, n);
            delete_lines(char_lines_array);
            break;
        case 4:
            n = read_int() - 1;
            lines_counter = read_int_lines(int_lines_array);
            sort_by_average(int_lines_array, lines_counter);
            write_int_line(int_lines_array, n);
            delete_int_lines(int_lines_array, lines_counter);
            break;
        case 5:
            scanf("%d %d %d", &rows, &cols, &n_triplets);
            n_triplets = read_sparse(triplet_array, n_triplets);
            read_vector(x, cols);
//            write_vector(x, cols);
            make_CSR(triplet_array, n_triplets, rows, V, C, R);
            multiply_by_vector(rows, V, C, R, x, y);
            write_vector(V, n_triplets);
            write_vector(C, n_triplets);
            write_vector(R, rows + 1);
            write_vector(y, rows);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}


