//
// Created by piotr on 17.04.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct
{
    int first;
    int second;
} pair;

// Add pair to existing relation if not already there
int add_relation(pair *relations, int n, pair new);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair *, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair *, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair *, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair *, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair *, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair *, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair *, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair *, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair *, int);

int find_max_elements(pair *, int, int *);

int find_min_elements(pair *, int, int *);

int get_domain(pair *, int, int *);

// Case 3:

int composition(pair *, int, pair *, int, pair *);

// Comparator for pair
int cmp_pair(const void *a, const void *b)
{
    pair *p1 = (pair *)a;
    pair *p2 = (pair *)b;
    if (p1->first != p2->first)
        return p1->first - p2->first;
    return p1->second - p2->second;
}

int cmp(const void *a, const void *b)
{
    int *p1 = (int *)a;
    int *p2 = (int *)b;
    return *p1 - *p2;
}

// int insert_int(int *tab, int n, int new_element) {
// }

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation)
{
    int n = 0, first, second;
    pair temp;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d %d", &first, &second);
        temp.first = first;
        temp.second = second;
        relation[i] = temp;
    }

    return n;
}

void print_int_array(int *array, int n)
{
    printf("%d\n", n);
    for (int i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");
}

int main(void)
{
    setbuf(stdout, 0);

    int to_do;
    pair relation[MAX_REL_SIZE];
    pair relation_2[MAX_REL_SIZE];
    pair comp_relation[MAX_REL_SIZE];
    int domain[MAX_REL_SIZE];
    int max_elements[MAX_REL_SIZE];
    int min_elements[MAX_REL_SIZE];

    scanf("%d", &to_do);
    int size = read_relation(relation);
    int ordered, size_2, n_domain;

    qsort(relation, size, sizeof(pair), cmp_pair);
    //    for (int i = 0; i < size; i++)
    //        printf("%d %d \n", relation[i].first, relation[i].second);

    switch (to_do)
    {
    case 1:
        printf("%d ", is_reflexive(relation, size));
        printf("%d ", is_irreflexive(relation, size));
        printf("%d ", is_symmetric(relation, size));
        printf("%d ", is_antisymmetric(relation, size));
        printf("%d ", is_asymmetric(relation, size));
        printf("%d\n", is_transitive(relation, size));
        break;
    case 2:
        ordered = is_partial_order(relation, size);
        n_domain = get_domain(relation, size, domain);
        printf("%d %d\n", ordered, is_total_order(relation, size));
        print_int_array(domain, n_domain);
        if (!ordered)
            break;
        int no_max_elements = find_max_elements(relation, size, max_elements);
        int no_min_elements = find_min_elements(relation, size, min_elements);
        print_int_array(max_elements, no_max_elements);
        print_int_array(min_elements, no_min_elements);
        break;
    case 3:
        size_2 = read_relation(relation_2);
        printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }

    return 0;
}

// Add pair to existing relation if not already there
int add_relation(pair *t, int n, pair new_pair)
{
    for (int i = 0; i < n; i++)
        if (cmp_pair(&t[i], &new_pair) == 0)
            return n;
    t[n] = new_pair;
    return n + 1;
}

int is_reflexive(pair *t, int n)
{
    pair temp;
    int x, prev = t[0].first - 1;
    for (int i = 0; i < n; i++)
    {
        x = t[i].first;
        if (x != prev)
        {
            temp.first = x;
            temp.second = x;
            if (NULL == bsearch(&temp, t, n, sizeof(pair), cmp_pair))
                return 0;
        }
        prev = x;
    }
    return 1;
}

int is_irreflexive(pair *t, int n)
{
    int prev = t[0].first - 1;
    for (int i = 0; i < n; i++)
    {
        if (t[i].first != prev)
            if (t[i].first == t[i].second)
                return 0;
        prev = t[i].first;
    }
    return 1;
}

int is_symmetric(pair *t, int n)
{
    int checked[n];
    //    for (int i = 0; i < n; i++) {
    //        checked[i] = 0;
    //    }
    for (int i = 0; i < n; i++)
    {
        if (!checked[i])
            for (int j = 0; j < n; j++)
            {
                if (t[i].first == t[j].second && t[i].second == t[j].first)
                {
                    checked[i] = 1;
                    checked[j] = 1;
                    break;
                }
            }
    }
    for (int i = 0; i < n; i++)
        if (!checked[i])
            return 0;
    return 1;
}

int is_antisymmetric(pair *t, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (t[i].first == t[j].second && t[i].second == t[j].first)
                if (t[i].first != t[i].second)
                    return 0;
        }
    }
    return 1;
}

int is_asymmetric(pair *t, int n)
{
    return is_antisymmetric(t, n) && is_irreflexive(t, n);
}

int is_transitive(pair *t, int n)
{
    int y, k;
    for (int i = 0; i < n; i++)
    {
        for (y = i + 1; y < n && t[y].first != t[i].second; y++)
        {
        }
        for (int j = y; j < n && t[j].first == t[i].second; j++)
        {
            for (k = i; k < n && t[k].first == t[i].first; k++)
            {
                if (t[k].second == t[j].second)
                    break;
            }
            if (k == n || t[k].first != t[i].first)
                return 0;
        }
    }
    return 1;
}

int is_partial_order(pair *t, int n)
{
    return is_reflexive(t, n) && is_antisymmetric(t, n) && is_transitive(t, n);
}

int is_total_order(pair *t, int n)
{
    return is_partial_order(t, n) && is_connected(t, n);
}

int is_connected(pair *t, int n)
{
    int checked[n];
    for (int i = 0; i < n; i++)
    {
        if (!checked[i])
            for (int j = 0; j < n; j++)
            {
                if (t[i].first == t[j].second && t[i].second == t[j].first)
                {
                    checked[i] = 1;
                    checked[j] = 1;
                    break;
                }
            }
    }
    for (int i = 0; i < n; i++)
    {
        if (!checked[i])
            return 0;
    }
    return 1;
}

int find_max_elements(pair *t, int n, int *maxs)
{
    int i = 0, max_candidate, j;
    int max_cnt = 0;
    while (i < n)
    {
        max_candidate = t[i].second;
        for (j = 0; j < n; j++)
        {
            if (t[j].first == max_candidate && t[j].second != max_candidate)
                break;
        }
        if (j == n)
        {
            maxs[max_cnt] = max_candidate;
            max_cnt++;
        }
        while (i < n && t[i].first == max_candidate)
            i++;
    }
    return max_cnt;
}

int find_min_elements(pair *t, int n, int *mins)
{
    int i = 0, min_candidate, j;
    int min_cnt = 0;
    while (i < n)
    {
        min_candidate = t[i].first;
        for (j = 0; j < n; j++)
        {
            if (t[j].second == min_candidate && t[j].first != min_candidate)
                break;
        }
        if (j == n)
        {
            mins[min_cnt] = min_candidate;
            min_cnt++;
        }
        while (i < n && t[i].first == min_candidate)
            i++;
    }
    return min_cnt;
}

int get_domain(pair *t, int n, int *domain)
{
    int visited[MAX_RANGE];
    for (int i = 0; i < MAX_RANGE; i++)
    {
        visited[i] = 0;
        domain[i] = MAX_RANGE + 1;
    }
    for (int i = 0; i < n; i++)
    {
        visited[t[i].first] = 1;
        visited[t[i].second] = 1;
    }
    int domain_cnt = 0;
    for (int i = 0; i < MAX_RANGE; i++)
    {
        if (visited[i])
        {
            domain[domain_cnt] = i;
            domain_cnt++;
        }
    }
    qsort(domain, domain_cnt, sizeof(int), cmp);
    return domain_cnt;
}

int composition(pair *t1, int n, pair *t2, int to_cmp, pair *added)
{
    pair new;
    int added_n = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < to_cmp; j++)
        {
            if (t1[i].second == t2[j].first)
            {
                new.first = t1[i].first;
                new.second = t2[j].second;
                added_n = add_relation(added, added_n, new);
            }
        }
    }
    return added_n;
}