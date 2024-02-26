//
// Created by piotr on 08.05.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 1

typedef struct
{
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX 30   // max. liczba artykułów
#define ART_MAX 15    // max. długość nazwy artykułu +1
#define RECORD_MAX 40 // max. długość rekordu (linii) w pliku

typedef struct
{
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compar, char *result)
{
    long long l = 0, r = nitems - 1;
    while (l <= r)
    {
        long long s = (l + r) / 2;
        void *s_ptr = (void *)base + s * size;
        int cmp_res = compar(s_ptr, key);
        if (cmp_res == 0)
        {
            *result = 1;
            return (void *)s_ptr;
        }
        else if (cmp_res > 0)
            r = s - 1;
        else
            l = s + 1;
    }
    *result = 0;
    return (void *)base + l * size;
}

void print_art(Food *p, size_t n, char *art)
{
    for (int i = 0; i < n; i++)
    {
        if (strcmp(p[i].art, art) == 0)
        {
            printf("%.2f %.2f %d.%d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
                   p[i].valid_date.year);
        }
    }
    printf("\n");
}

Food *add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new)
{
    char result;
    Food *poz = bsearch2(new, tab, (size_t)*np, sizeof(Food), compar, &result);
    if (result == 1)
    {
        poz->amount = poz->amount + new->amount;
    }
    else
    {
        if ((size_t)*np + 1 == tab_size)
            return NULL;

        memmove(poz + 1, poz, *np * sizeof(Food));
        memcpy(poz, new, 1 * sizeof(Food));
        (*np)++;
    }
    return poz;
}

int cmp_dates(const void *p1, const void *p2)
{
    Food *a = (Food *)p1;
    Food *b = (Food *)p2;
    return (a->valid_date.day + a->valid_date.month * 30 + a->valid_date.year * 365) -
           (b->valid_date.day + b->valid_date.month * 30 + b->valid_date.year * 365);
}

int cmp(const void *p1, const void *p2)
{
    Food *a = (Food *)p1;
    Food *b = (Food *)p2;
    int str_cmp_res = strcmp(a->art, b->art);
    if (str_cmp_res != 0)
        return str_cmp_res;
    int price_cmp = (int)(a->price * 100) - (int)(b->price * 100);
    if (price_cmp != 0)
        return price_cmp;
    return cmp_dates(p1, p2);
}

int read_stream(Food *tab, size_t size, int no, FILE *stream)
{
    int np = 0;
    int d, m, y;
    for (int i = 0; i < no; i++)
    {
        Food new;
        if (TEST)
            fscanf(stream, "%s %f %f %d.%d.%d", new.art, &new.price, &new.amount, &d, &m, &y);
        else
            fscanf(stream, "%s %f %f %d %d %d", new.art, &new.price, &new.amount, &d, &m, &y);
        Date new_date = {d, m, y};
        new.valid_date = new_date;
        add_record(tab, size, &np, cmp, &new);
    }
    return np;
}

int read_stream0(Food *tab, size_t size, int no, FILE *stream)
{
    return read_stream(tab, size, no, stream);
}

float value(Food *food_tab, size_t n, Date curr_date, int anticip)
{
    qsort(food_tab, n, sizeof(Food), cmp_dates);
    Food curr_food;
    curr_food.valid_date = curr_date;
    for (int i = 0; i < n; i++)
    {
        printf("%s %.2f %d.%d.%d\n", food_tab[i].art, food_tab[i].amount, food_tab[i].valid_date.day,
               food_tab[i].valid_date.month,
               food_tab[i].valid_date.year);
    }
    float sum_amount = 0;
    for (int i = (int)n - 1; i >= 0; i--)
    {
        int days_between = cmp_dates(&food_tab[i], &curr_food);
        if (days_between == anticip)
            sum_amount += food_tab[i].price * food_tab[i].amount;
    }
    return sum_amount;
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX 20 // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex
{
    F,
    M
};
enum BOOL
{
    no,
    yes
};

struct Bit_data
{
    enum Sex sex : 1;
    enum BOOL pretendent : 1; // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct
{
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct
{
    char *par_name;
    int index_first;
    int index_last;
} Parent; // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = {28, 10, 2011}; // Data zmiany kolejności sukcesji

int person_cmp_parent(const void *_a, const void *_b)
{
    Person a = *((Person *)_a);
    Person b = *((Person *)_b);

    if (a.parent == NULL)
        return 1;
    if (b.parent == NULL)
        return -1;

    return strcmp(a.parent, b.parent);
}

int parent_cmp(const void *_a, const void *_b)
{
    Parent a = *((Parent *)_a);
    Parent b = *((Parent *)_b);

    return strcmp(a.par_name, b.par_name);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size)
{
    int no_parents = 0;

    for (int i = 0; i < size; i++)
    {
        int found = 0;

        for (int u = 0; u < no_parents; u++)
        {
            if (strcmp(pers_tab[i].name, idx_tab[u].par_name) == 0)
            {
                found = 1;
                break;
            }
        }

        if (!found)
        {
            idx_tab[no_parents].par_name = pers_tab[i].name;

            Person key = {
                .parent = pers_tab[i].name};

            Person *address = bsearch(&key, pers_tab, size, sizeof(Person), person_cmp_parent);

            if (address == NULL)
                continue;

            int index = (int)(address - pers_tab);
            int tmp = index;

            while (tmp > 0 && person_cmp_parent(&pers_tab[tmp], &pers_tab[tmp - 1]) == 0)
                tmp--;

            idx_tab[no_parents].index_first = tmp;
            tmp = index;

            while (tmp < size - 1 && person_cmp_parent(&pers_tab[tmp], &pers_tab[tmp + 1]) == 0)
                tmp++;

            idx_tab[no_parents].index_last = tmp;

            no_parents++;
        }
    }

    qsort(idx_tab, no_parents, sizeof(Parent), parent_cmp);

    return no_parents;
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents)
{
    int index = 0;

    Person tmp[33];

    while (index < size)
    {
        Parent key = {.par_name = person_tab[index].name};
        Parent *address = bsearch(&key, idx_tab, no_parents, sizeof(Parent), parent_cmp);

        if (address != NULL)
        {
            int to_copy = address->index_last - address->index_first + 1;

            memmove(tmp, &person_tab[address->index_first], to_copy * sizeof(Person));
            memmove(&person_tab[index + to_copy + 1], &person_tab[index + 1],
                    ((address->index_first) - index - 1) * sizeof(Person));
            memmove(&person_tab[index + 1], tmp, to_copy * sizeof(Person));

            for (int i = 0; i < no_parents; i++)
            {
                if (idx_tab[i].index_first < address->index_first)
                {
                    idx_tab[i].index_first += to_copy;
                    idx_tab[i].index_last += to_copy;
                }
            }
        }

        index += 1;
    }
}

int cleaning(Person *person_tab, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (person_tab[i].bits.pretendent == no)
        {
            memmove(&person_tab[i], &person_tab[i + 1], (n - i) * sizeof(Person));
            i--;
            n--;
        }
    }

    return n;
}

void print_person(const Person *p)
{
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n)
{
    for (int i = 1; i <= n; ++i, ++person_tab)
        printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
}

int compare(const void *_a, const void *_b)
{
    Person a = *((Person *)_a);
    Person b = *((Person *)_b);

    if (a.parent == NULL)
        return -1;
    if (b.parent == NULL)
        return 1;

    int parent = strcmp(a.parent, b.parent);

    if (parent < 0)
        return -1;
    else if (parent > 0)
        return 1;
    else
    {
        int born_date = compare_date(&a.born, &b.born);

        if (compare_date(&a.born, &primo_date) < 0 && compare_date(&b.born, &primo_date) < 0)
        {
            if (a.bits.sex == M && b.bits.sex == F)
                return -1;
            else if (a.bits.sex == F && b.bits.sex == M)
                return 1;
        }

        if (born_date < 0)
            return -1;
        else if (born_date > 0)
            return 1;
        else
        {
            if (a.bits.sex > b.bits.sex)
                return 1;
            else if (a.bits.sex < b.bits.sex)
                return -1;
            else
                return 0;
        }
    }
}

int create_list(Person *person_tab, int n)
{
    qsort(person_tab, n, sizeof(Person), compare);

    Parent parents_tab[33];

    int no_parents = fill_indices_tab(parents_tab, person_tab, n);

    persons_shiftings(person_tab, n, parents_tab, no_parents);

    return cleaning(person_tab, n);
}

////////////////////////////////////////////////////////////////

int main(void)
{
    setbuf(stdout, 0);
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33] = {
        {"Charles III", M, no, 14, 11, 1948, "Elizabeth II"},
        {"Anne", F, yes, 15, 8, 1950, "Elizabeth II"},
        {"Andrew", M, yes, 19, 2, 1960, "Elizabeth II"},
        {"Edward", M, yes, 10, 3, 1964, "Elizabeth II"},
        {"David", M, yes, 3, 11, 1961, "Margaret"},
        {"Sarah", F, yes, 1, 5, 1964, "Margaret"},
        {"William", M, yes, 21, 6, 1982, "Charles III"},
        {"Henry", M, yes, 15, 9, 1984, "Charles III"},
        {"Peter", M, yes, 15, 11, 1977, "Anne"},
        {"Zara", F, yes, 15, 5, 1981, "Anne"},
        {"Beatrice", F, yes, 8, 8, 1988, "Andrew"},
        {"Eugenie", F, yes, 23, 3, 1990, "Andrew"},
        {"James", M, yes, 17, 12, 2007, "Edward"},
        {"Louise", F, yes, 8, 11, 2003, "Edward"},
        {"Charles", M, yes, 1, 7, 1999, "David"},
        {"Margarita", F, yes, 14, 5, 2002, "David"},
        {"Samuel", M, yes, 28, 7, 1996, "Sarah"},
        {"Arthur", M, yes, 6, 5, 2019, "Sarah"},
        {"Georg", M, yes, 22, 7, 2013, "William"},
        {"George VI", M, no, 14, 12, 1895, NULL},
        {"Charlotte", F, yes, 22, 5, 2015, "William"},
        {"Louis", M, yes, 23, 4, 2018, "William"},
        {"Archie", M, yes, 6, 5, 2019, "Henry"},
        {"Lilibet", F, yes, 4, 6, 2021, "Henry"},
        {"Savannah", F, yes, 29, 12, 2010, "Peter"},
        {"Isla", F, yes, 29, 3, 2012, "Peter"},
        {"Mia", F, yes, 17, 1, 2014, "Zara"},
        {"Lena", F, yes, 18, 6, 2018, "Zara"},
        {"Elizabeth II", F, no, 21, 4, 1925, "George VI"},
        {"Margaret", F, no, 21, 8, 1930, "George VI"},
        {"Lucas", M, yes, 21, 3, 2021, "Zara"},
        {"Sienna", F, yes, 18, 9, 2021, "Beatrice"},
        {"August", M, yes, 9, 2, 2021, "Eugenie"}};

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE *file;
    if (TEST)
        printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do)
    {
    case 1: // bsearch2
        if (TEST)
            printf("Wpisz liczbe linii danych: ");
        scanf("%d", &no);
        getchar();
        if (TEST)
            file = stdin;
        else
        {
            scanf("%s", buff);
            file = fopen(buff, "r");
            //            file = fopen("foods0.txt","r");
            if (file == NULL)
            {
                printf("Error 1\n");
                break;
            }
        }
        if (TEST)
            printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
        n = read_stream(food_tab, FOOD_MAX, no, file);
        if (TEST)
            printf("Wpisz nazwe artykulu: ");
        scanf("%s", buff);
        print_art(food_tab, n, buff);
        break;
    case 2: // proste sortowanie struktur
        if (TEST)
            printf("Wpisz liczbe linii danych: ");
        scanf("%d", &no);
        getchar();
        if (TEST)
            file = stdin;
        else
        {
            scanf("%s", buff);
            file = fopen(buff, "r");
            //            file = fopen("foods0.txt","r");
            if (file == NULL)
            {
                printf("Error 1\n");
                break;
            }
        }
        if (TEST)
            printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
        n = read_stream0(food_tab, FOOD_MAX, no, file);
        Date curr_date;
        int anticip;
        if (TEST)
            printf("Wpisz date odniesienia dd mm yyyy: ");
        scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
        if (TEST)
            printf("Wpisz roznice dni: ");
        scanf("%d", &anticip);
        printf("%.2f\n", value(food_tab, n, curr_date, anticip));
        break;
    case 3: // sukcesja
        if (TEST == 1)
            printf("Wpisz pozycję w kolejce (liczona od 1): ");
        scanf("%d", &no);
        int no_persons = sizeof(person_tab) / sizeof(Person);
        no_persons = create_list(person_tab, no_persons);
        if (TEST)
            print_persons(person_tab, no_persons);
        else
            print_person(person_tab + no - 1);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
