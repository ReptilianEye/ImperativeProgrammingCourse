//
// Created by piotr on 27.06.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR -1
#define PROGRAM_ERROR -3

int cmp(void *p1, void *p2)
{
    int a = *(int *)p1;
    int b = *(int *)p2;
    return a - b;
}

size_t insert(void *data, size_t n, size_t index, void *value)
{
    int element_size = sizeof(int);
    memcpy((char *)data + (index + 1) * element_size,
           (char *)(data) + index * element_size, element_size * (n - index));
    memcpy(((char *)(data) + index * element_size), (char *)value, element_size);
    return n + 1;
}

void dump_input(int *input, size_t n)
{
    for (int i = 0; i <= n; i++)
    {
        printf("%d ", *(input + i * sizeof(int)));
    }
    printf("\n");
}

size_t stream_to_list(FILE *stream, int *input)
{
    size_t n = 0;
    char *p, buf[BUFFER_SIZE];
    char delimits[] = " \r\t\n.;,?!-";
    while (fgets(buf, BUFFER_SIZE, stream))
    {
        p = strtok(buf, delimits);
        while (p)
        {
            int num = atoi(p);
            *(input + n * sizeof(int)) = num;
            n++;
            p = strtok(NULL, delimits);
        }
    }
    return n - 1;
}
typedef struct _matrix
{
    int rows;
    int cols;
    double **val;
    double *storage;
} Matrix;
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr)
        return ptr;
    return NULL;
}

int create_matrix(Matrix *pmatrix, int rows, int cols)
{
    pmatrix->rows = rows;
    pmatrix->cols = cols;
    pmatrix->val = (double **)safe_malloc(rows * sizeof(double *));
    if (pmatrix->val == NULL)
        return 0;
    pmatrix->storage = (double *)safe_malloc(rows * cols * sizeof(double));
    if (pmatrix->storage)
        return 0;
    return 1;
}
Matrix random_matrix(int rows, int cols)
{
    Matrix matrix;
    create_matrix(&matrix, rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            *(matrix.val[i] + j) = (double)rand() / RAND_MAX;
        }
    }

    return matrix;
}
void copy_matrix(Matrix *ptrg, const Matrix *psrc)
{
    int rows = psrc->rows;
    int cols = psrc->cols;
    create_matrix(ptrg, rows, cols);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            *(ptrg->val[i] + j) = *(psrc->val[i] + j);
        }
    }
}
void multiply_by(Matrix *pm, double v)
{
    for (int i = 0; i < pm->rows; i++)
    {
        for (int j = 0; j < pm->cols; j++)
        {
            *(pm->val[i] + j) *= v;
        }
    }
}
int add(Matrix *presult, const Matrix *pm1, const Matrix *pm2)
{
    if (pm1->rows != pm2->rows || pm1->cols != pm2->cols)
    {
        return 0;
    }

    for (int i = 0; i < pm1->rows; i++)
    {
        for (int j = 0; j < pm1->cols; j++)
        {
            *(presult->val[i] + j) = *(pm1->val[i] + j) + *(pm2->val[i] + j);
        }
    }

    return 1;
}
size_t glob_columns;
int compar(const void *a, const void *b)
{
    double *rowA = (double *)a;
    double *rowB = (double *)b;

    double sumA = 0.0;
    double sumB = 0.0;

    for (int i = 0; i < glob_columns; i++)
    {
        sumA += *(rowA + i);
        sumB += *(rowB + i);
    }

    double avgA = sumA / glob_columns;
    double avgB = sumB / glob_columns;

    if (avgA < avgB)
    {
        return -1;
    }
    else if (avgA > avgB)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
enum Drive_type
{
    combustion,
    electric,
    d_ndef
};
enum Fuel_type
{
    gasoline,
    diesel,
    gas,
    hydrogen,
    f_d_ndef
};

enum Battery_type
{
    Li_Ion,
    Na_Ion,
    NiMH,
    b_d_ndef
};
typedef struct
{
    char emmision[15];
    enum Fuel_type fuel_t;
} Comb_data;

typedef struct
{
    enum Battery_type bat_t;
    float capacity;
} El_data;
typedef struct
{
    int d;
    int m;
    int y;
} Date;

typedef struct
{
    char owner[30]; // zawiera imię i nazwisko właściciela
    Date valid_date;
    enum Drive_type d_type;
    union
    {
        Comb_data cs;
        El_data es;
    } drv_union;
} Vehicle;
int cmp_dates(Date a, Date b)
{
    int year = a.y - b.y;
    if (year != 0)
        return year;
    int month = a.m - b.m;
    if (month != 0)
        return month;
    return a.d - b.d;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, int **pdelayed_owners)
{
    int count = 0;
    int *delayed_owners = NULL;

    for (size_t i = 0; i < size; i++)
    {
        if (cmp_dates(tv[i].valid_date, *base_date) < 0)
        {
            count++;
            int *temp = realloc(delayed_owners, count * sizeof(int));
            if (temp == NULL)
            {
                free(delayed_owners);
                return -1;
            }
            delayed_owners = temp;
            delayed_owners[count - 1] = i;
        }
    }

    *pdelayed_owners = delayed_owners;
    return count;
}
void print_owners(const Vehicle *tv, int *t_ind, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%s\n", tv[t_ind[i]].owner);
    }
}
Vehicle *create_vehicle()
{
    Vehicle *vehicle = malloc(sizeof(Vehicle));
    if (vehicle == NULL)
    {
        return NULL; // Błąd przy alokacji pamięci
    }

    strcpy(vehicle->owner, "");
    vehicle->valid_date.d = 1;
    vehicle->valid_date.m = 1;
    vehicle->valid_date.y = 1970;
    vehicle->d_type = d_ndef;

    return vehicle;
}
void print_drive_data(const Vehicle *v)
{
    printf("Naped: ");
    switch (v->d_type)
    {
    case combustion:
        printf("spalinowy, emisja: %s, paliwo: ", v->drv_union.cs.emmision);
        switch (v->drv_union.cs.fuel_t)
        {
        case gasoline:
            printf("gasoline");
            break;
        case diesel:
            printf("diesel");
            break;
        case gas:
            printf("gas");
            break;
        case hydrogen:
            printf("hydrogen");
            break;
        case f_d_ndef:
            break;
        }
        break;
    case electric:
        printf("el, bateria: ");
        switch (v->drv_union.es.bat_t)
        {
        case Li_Ion:
            printf("Li-Ion");
            break;
        case Na_Ion:
            printf("Na-Ion");
            break;
        case NiMH:
            printf("NiMH");
            break;
        case b_d_ndef:
            break;
        }
        printf(", pojemnosc: %.2fkWh", v->drv_union.es.capacity);
        break;
    case d_ndef:
        break;
    }
    printf("\n");
}
int int_put_data(Vehicle *v, enum Drive_type type, const void *data)
{
    if (v->d_type == d_ndef || v->d_type == type)
    {
        switch (type)
        {
        case combustion:
            memcpy(&v->drv_union.cs, data, sizeof(Comb_data));
            break;
        case electric:
            memcpy(&v->drv_union.es, data, sizeof(El_data));
            break;
        case d_ndef:
            break;
        }
        return 1;
    }
    return 0;
}

int main()
{
    Matrix M;
    setbuf(stdout, 0);
    qsort(M.val, M.rows, sizeof(double *), compar);

    FILE *stream = stdin;
    int *input = safe_malloc(sizeof(int) * 100);
    size_t n = stream_to_list(stream, input);
    dump_input(input, n);
}