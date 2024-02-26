//
// Created by piotr on 15.03.2023.
//
#include <stdio.h>
#include <math.h>

#define TEST 1 // 1 dla testowania, 0 dla oceny automatycznej

// Deklaracje (prototypy) funkcji:
void swap(int *a, int *b);

double stack_push(double x);

double stack_pop(void);

int stack_state(void);

double desk_push(int how_many);

int desk_pop(int how_many);

int desk_state(void);

void desk_print(void);

double cbuff_push(int cli_nr);

int cbuff_pop(void);

int cbuff_state(void);

void cbuff_print(void);

///////////////////////

int main(void)
{
    setbuf(stdout, 0);
    int to_do, i, n, ans;
    int nr_klienta, kod_operacji;
    double x, f_ans;
    if (TEST)
        printf("Nr testu = ");
    scanf("%d", &to_do);
    switch (to_do)
    {
    case 1:
        if (TEST)
            printf("sekwencja operacji (w jednej linii, oddzielone spacjami):\n"); // 0 podaje stan stosu i konczy sekwencje operacji
        do
        {
            scanf("%lf", &x);
            if (x > 0)
            {
                f_ans = stack_push(x);
                if (f_ans == INFINITY)
                    printf("%f ", INFINITY);
            }
            else
            {
                if (x < 0)
                    printf("%.2f ", stack_pop());
                else
                    printf("\n%d\n", stack_state());
            }
        } while (x != 0.0);
        break;
    case 2:
        //  kolejka kolejnych liczb naturlnych > 0 (naplywajacych grupowo) - implementacja w tablicy z przesunieciami
        if (TEST)
            printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n"); // 0 podaje stan kolejki i konczy symulacje kolejki
        do
        {
            scanf("%d", &n);
            if (n > 0)
            {
                f_ans = desk_push(n);
                if (f_ans == INFINITY)
                    printf("%f ", INFINITY);
            }
            else
            {
                if (n < 0)
                {
                    ans = desk_pop(-n);
                    if (ans < 0)
                        printf("-1 ");
                }
                else
                {
                    printf("\n%d\n", desk_state());
                    desk_print();
                }
            }
        } while (n != 0);
        break;
    case 3:
        //  kolejka kolejnych liczb naturalnych > 0 (naplywajacych pojedynczo - implementacja w buforze cyklicznym
        if (TEST)
            printf("we/wy do/z kolejki (w jednej linii, oddzielone spacjami):\n"); // 0 podaje stan kolejki i konczy symulacje kolejki
        nr_klienta = 0;
        do
        {
            scanf("%d", &kod_operacji);
            if (kod_operacji > 0)
            {
                f_ans = cbuff_push(++nr_klienta);
                if (f_ans == INFINITY)
                    printf("%f ", INFINITY);
            }
            else
            {
                if (kod_operacji < 0)
                {
                    printf("%d ", cbuff_pop());
                    //                    ans = hand_pop();
                    //                    if(ans < 0) printf("%f ",NAN);
                    //                    else        printf("%d ",ans);
                }
                else
                {
                    printf("\n%d\n", cbuff_state());
                    cbuff_print();
                }
            }
        } while (kod_operacji != 0);
        break;
    default:
        if (TEST)
            printf("NOTHING TO DO!\n");
    }
    return 0;
}

///////////////////////////////////////////////////////

// Definicje funkcji i zmiennych o zasięgu plikowym (file scope):

// Stos

#define STACK_SIZE 10

double stack[STACK_SIZE];
int top = 0; // jawne zerowanie nie jest konieczne

double stack_push(double x)
{
    if (top == STACK_SIZE)
        return INFINITY;
    top++;
    stack[top - 1] = x;
    return 0;
}

double stack_pop(void)
{
    if (top == 0)
        return NAN;
    top--;
    return stack[top];
}

int stack_state(void)
{
    return top;
}

// Kolejka z przesuwaniem w tablicy

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0; // 1. klient dostanie nr = 1

double desk_push(int how_many)
{
    while (in < QUEUE_SIZE && how_many > 0)
    {
        queue[in] = curr_nr + 1;
        curr_nr++;
        in++;
        how_many--;
    }
    if (how_many > 0)
    {
        curr_nr = curr_nr + how_many;
        return INFINITY;
    }
    return 0;
}

void swap(int *a, int *b)
{
    //    *a = *a + *b;
    //    *b = *a - *b;
    //    *a = *a - *b;
    int temp = *a;
    *a = *b;
    *b = temp;
}

int desk_pop(int how_many)
{
    int i = 0, j = 0;
    if (how_many > in)
    {
        for (i = 0; i < QUEUE_SIZE; i++)
        {
            queue[i] = 0;
        }
        in = 0;
        return -1;
    }
    while (i < in && i < how_many)
    {
        queue[i] = 0;
        i++;
    }
    if (i == how_many)
    {
        while (i < in)
        {
            swap(&queue[i], &queue[j]);
            i++;
            j++;
        }
    }
    in = j;
    return in;
}

int desk_state(void)
{
    return in;
}

void desk_print(void)
{
    for (int i = 0; i < in; i++)
    {
        printf("%d ", queue[i]);
    }
}

// Kolejka w buforze cyklicznym

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;

double cbuff_push(int cli_nr)
{
    if (len == out && cbuff[len] != 0)
        return INFINITY;
    cbuff[len] = cli_nr;
    len = (len + 1) % CBUFF_SIZE;
}

int cbuff_pop(void)
{
    if (out == len && cbuff[len] == 0)
        return -1;
    else
    {
        int c_cout = cbuff[out];
        cbuff[out] = 0;
        out = (out + 1) % CBUFF_SIZE;
        return c_cout;
    }
}

int cbuff_state(void)
{
    if (out < len)
        return len - out;
    if (cbuff[out] == 0)
        return 0;
    if (out == len && cbuff[len] != 0)
        return CBUFF_SIZE;
    return CBUFF_SIZE - (out - len);
}

void cbuff_print(void)
{
    int i = out;
    if (cbuff[out] != 0)
        do
        {
            printf("%d ", cbuff[i]);
            i = (i + 1) % CBUFF_SIZE;
        } while (i != len);
}