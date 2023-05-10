//
// Created by piotr on 08.05.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TEST 1

typedef struct {
    int day;
    int month;
    int year;
} Date;

/////////////////////////////////////////////////////////////////
// 8.1 funkcja bsearch2

#define FOOD_MAX  30   // max. liczba artykułów
#define ART_MAX   15   // max. długość nazwy artykułu +1
#define RECORD_MAX 40  // max. długość rekordu (linii) w pliku

typedef struct {
    char art[ART_MAX];
    float price;
    float amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

void *bsearch2(const void *key, void *base, size_t nitems, size_t size, ComparFp compar, char *result) {
    int l = 0;
    int r = (int) nitems - 1;
    int s = (r + l) / 2;
    int res;
    int found = 0;
    while (l <= r) {
        res = compar(&base[s], &key);
        if (res == 0) {
            found = 1;
            break;
        }
        if (res > 0) {
            r = s - 1;
        } else
            l = s + 1;
        s = (r + l) / 2;

    }
    if (found || l == r) {
        memcpy(result, "FOUND", 6);
    } else {
        memcpy(result, "NOT FOUND", 11);
    }
    return (void *) &base[l];


}

void print_art(Food *p, size_t n, char *art) {
    for (int i = 0; i < n; i++) {
//        if (strcmp(p[i].art, art) == 0) {
        printf("%f %f %d.%d.%d", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month,
               p[i].valid_date.year);
//        }
    }
}

Food *add_record(Food *tab, size_t tab_size, int *np, ComparFp compar, Food *new) {
    char result[10];
    Food *poz = bsearch2(new, tab, (size_t) *np, sizeof(Food), compar, result);
    if (strcmp(result, "FOUND") == 0) {
        poz->amount = poz->amount + new->amount;
    } else {
        if ((size_t) np + 1 == tab_size)
            return NULL;
        memmove(poz + 1, poz, tab_size * sizeof(Food));
        memcpy(poz, new, sizeof(Food));
        *np = *np + 1;
    }
    return poz;
}

int cmp(const void *p1, const void *p2) {
    Food *a = (Food *) p1;
    Food *b = (Food *) p2;
//    const Food *a = p1;
//    const Food *b = p2;
    int price_cmp = (int) a->price * 100 - (int) b->price * 100;
    if (price_cmp != 0)
        return price_cmp;
    int date_cmp = (a->valid_date.day + a->valid_date.month * 30 + a->valid_date.year * 365) -
                   (b->valid_date.day + b->valid_date.month * 30 + b->valid_date.year * 365);
    if (date_cmp != 0)
        return date_cmp;

    return strcmp(a->art, b->art);

}

int read_stream(Food *tab, size_t size, int no, FILE *stream) {
    int np = 0;
    int d, m, y;
    for (int i = 0; i < no; i++) {
        Food new;
        if (TEST)
            fscanf(stream, "%s %f %f %d %d %d", new.art, &new.price, &new.amount, &d, &m, &y);
        else
            fscanf(stream, "%s %f %f %d.%d.%d", new.art, &new.price, &new.amount, &d, &m, &y);
        Date new_date = {d, m, y};
        new.valid_date = new_date;
        add_record(tab, size, &np, cmp, &new);
    }
    return np;
}

int read_stream0(Food *tab, size_t size, int no, FILE *stream) {
}

float value(Food *food_tab, size_t n, Date curr_date, int anticip) {
}

/////////////////////////////////////////////////////////////////
// 8.3 Sukcesja

#define CHILD_MAX  20   // maksymalna liczba przesuwanej w tablicy grupy osób (elementów tablicy)

enum Sex {
    F, M
};
enum BOOL {
    no, yes
};

struct Bit_data {
    enum Sex sex: 1;
    enum BOOL pretendent: 1;   // =no - nie pretenduje (panuje albo nie żyje) ale jest w zbiorze dla spójności relacji.
};

typedef struct {
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct {
    char *par_name;
    int index;
    int childrens;
} Parent;    // strukturę można rozbudować o pole liczby dzieci

const Date primo_date = {28, 10, 2011}; // Data zmiany kolejności sukcesji

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
}

void persons_shiftings(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
}

int cleaning(Person *person_tab, int n) {
}

void print_person(const Person *p) {
    printf("%s\n", p->name);
}

void print_persons(const Person *person_tab, int n) {
    for (int i = 1; i <= n; ++i, ++person_tab) printf("%2d %12s %s\n", i, person_tab->name, person_tab->parent);
    return;
}

int create_list(Person *person_tab, int n) {
}

////////////////////////////////////////////////////////////////

int main(void) {
    setbuf(stdout, 0);
    // Wszyscy potomkowie Jerzego VI (w kolejności przypadkowej):
    Person person_tab[33] = {
            {"Charles III",  M, no,  14, 11, 1948, "Elizabeth II"},
            {"Anne",         F, yes, 15, 8,  1950, "Elizabeth II"},
            {"Andrew",       M, yes, 19, 2,  1960, "Elizabeth II"},
            {"Edward",       M, yes, 10, 3,  1964, "Elizabeth II"},
            {"David",        M, yes, 3,  11, 1961, "Margaret"},
            {"Sarah",        F, yes, 1,  5,  1964, "Margaret"},
            {"William",      M, yes, 21, 6,  1982, "Charles III"},
            {"Henry",        M, yes, 15, 9,  1984, "Charles III"},
            {"Peter",        M, yes, 15, 11, 1977, "Anne"},
            {"Zara",         F, yes, 15, 5,  1981, "Anne"},
            {"Beatrice",     F, yes, 8,  8,  1988, "Andrew"},
            {"Eugenie",      F, yes, 23, 3,  1990, "Andrew"},
            {"James",        M, yes, 17, 12, 2007, "Edward"},
            {"Louise",       F, yes, 8,  11, 2003, "Edward"},
            {"Charles",      M, yes, 1,  7,  1999, "David"},
            {"Margarita",    F, yes, 14, 5,  2002, "David"},
            {"Samuel",       M, yes, 28, 7,  1996, "Sarah"},
            {"Arthur",       M, yes, 6,  5,  2019, "Sarah"},
            {"Georg",        M, yes, 22, 7,  2013, "William"},
            {"George VI",    M, no,  14, 12, 1895, NULL},
            {"Charlotte",    F, yes, 22, 5,  2015, "William"},
            {"Louis",        M, yes, 23, 4,  2018, "William"},
            {"Archie",       M, yes, 6,  5,  2019, "Henry"},
            {"Lilibet",      F, yes, 4,  6,  2021, "Henry"},
            {"Savannah",     F, yes, 29, 12, 2010, "Peter"},
            {"Isla",         F, yes, 29, 3,  2012, "Peter"},
            {"Mia",          F, yes, 17, 1,  2014, "Zara"},
            {"Lena",         F, yes, 18, 6,  2018, "Zara"},
            {"Elizabeth II", F, no,  21, 4,  1925, "George VI"},
            {"Margaret",     F, no,  21, 8,  1930, "George VI"},
            {"Lucas",        M, yes, 21, 3,  2021, "Zara"},
            {"Sienna",       F, yes, 18, 9,  2021, "Beatrice"},
            {"August",       M, yes, 9,  2,  2021, "Eugenie"}
    };

    int to_do, no;
    size_t size, n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    FILE *file;
    if (TEST) printf("Wpisz nr zadania (1 - 3) ");
    scanf("%d", &to_do);

    switch (to_do) {
        case 1:  // bsearch2
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d", &no);
            getchar();
            if (TEST) file = stdin;
            else {
                scanf("%s", buff);
                file = fopen(buff, "r");
//            file = fopen("foods0.txt","r");
                if (file == NULL) {
                    printf("Error 1\n");
                    break;
                }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
            n = read_stream(food_tab, FOOD_MAX, no, file);
            if (TEST) printf("Wpisz nazwe artykulu: ");
            scanf("%s", buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // proste sortowanie struktur
            if (TEST) printf("Wpisz liczbe linii danych: ");
            scanf("%d", &no);
            getchar();
            if (TEST) file = stdin;
            else {
                scanf("%s", buff);
                file = fopen(buff, "r");
//            file = fopen("foods0.txt","r");
                if (file == NULL) {
                    printf("Error 1\n");
                    break;
                }
            }
            if (TEST) printf("W %d liniach wpisuj dane: nazwa cena ilosc dd mm yyyy: \n", no);
            n = read_stream0(food_tab, FOOD_MAX, no, file);
            Date curr_date;
            int anticip;
            if (TEST) printf("Wpisz date odniesienia dd mm yyyy: ");
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            if (TEST) printf("Wpisz roznice dni: ");
            scanf("%d", &anticip);
            printf("%.2f\n", value(food_tab, n, curr_date, anticip));
            break;
        case 3: // sukcesja
            if (TEST == 1) printf("Wpisz pozycję w kolejce (liczona od 1): ");
            scanf("%d", &no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            no_persons = create_list(person_tab, no_persons);
            if (TEST) print_persons(person_tab, no_persons);
            else print_person(person_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}

