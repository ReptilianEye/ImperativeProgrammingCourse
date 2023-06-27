//
// Created by piotr on 20.06.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
    int *data;
    size_t array_size;
    struct Node *next;
    struct Node *prev;
} Node;

// doubly linked list
typedef struct List {
    Node *head;
    Node *tail;
} List;

// iterator
typedef struct iterator {
    struct Node *node_ptr;
    size_t position;
} iterator;

// forward initialization
iterator begin(Node *head) {
    iterator it = {head, 0};
    return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node *tail) {
    iterator it = {tail, tail->array_size};
    return it;
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) return ptr;
    exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
    void *ptr = realloc(old_ptr, size);
    if (ptr) return ptr;
    free(old_ptr);
    exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
    Node *node = safe_malloc(sizeof(Node));
    node->data = data;
    node->array_size = array_size;
    node->next = next;
    node->prev = prev;
    return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
    list->head = create_node(NULL, 0, NULL, NULL);
    list->tail = create_node(NULL, 0, NULL, list->head);
    list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
    Node *new_node = create_node(data, array_size, list->tail, list->tail->prev);
    list->tail->prev->next = new_node;
    list->tail->prev = new_node;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator *itr, size_t n) {
    int counter = (int) n;
    while (counter >= itr->node_ptr->array_size) {
        counter = counter - (int) itr->node_ptr->array_size;
        itr->node_ptr = itr->node_ptr->next;
    }
    while (counter > 0) {
        itr->position = itr->position + 1;
        counter = counter - 1;
    }

}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
    iterator itr = {list->head->next, 0};
    iterator *x = &itr;
    skip_forward(x, n - 1);
    return x->node_ptr->data[x->position];
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator *itr, size_t n) {
    int counter = (int) n;
    while (counter >= itr->node_ptr->array_size) {
        counter = counter - (int) itr->node_ptr->array_size;
        itr->node_ptr = itr->node_ptr->prev;
    }
    itr->position = (itr->node_ptr->array_size) - 1;
    while (counter > 0) {
        itr->position = itr->position - 1;
        counter = counter - 1;
    }
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
    iterator itr = {list->tail->prev, list->tail->prev->array_size};
    iterator *x = &itr;
    skip_backward(x, n - 1);
    return x->node_ptr->data[x->position];
}

void remove_node(Node *node_ptr) {
    free(node_ptr->data);
    free(node_ptr);
}

void set_new_array(iterator *itr, int *new_data) {
    itr->node_ptr->data = new_data;
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
    iterator x = {list->head->next, 0};
    iterator *itr = &x;
    skip_forward(itr, n - 1);
    if (itr->node_ptr->array_size == 1) {
        itr->node_ptr->prev->next = itr->node_ptr->next;
        itr->node_ptr->next->prev = itr->node_ptr->prev;
    } else {
        Node *new_node = malloc(sizeof(Node));
        new_node->array_size = itr->node_ptr->array_size - 1;
//        int new_array[itr->node_ptr->array_size-1];
        int *new_array = safe_malloc((new_node->array_size) * sizeof(int));
        new_node->data = new_array;
        int counter = 0;
        for (int i = 0; i < itr->node_ptr->array_size; ++i) {
            if (counter != (int) itr->position) {
                new_node->data[counter] = itr->node_ptr->data[i];
                counter = counter + 1;
            }
        }
        new_node->prev = itr->node_ptr->prev;
        new_node->next = itr->node_ptr->next;
        itr->node_ptr->prev->next = new_node;
        itr->node_ptr->next->prev = new_node;
    }
}

// return the number of digits of number n
size_t digits(int n) {
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
    for (Node *node = list->head->next; node != list->tail; node = node->next) {
        printf("-> ");
        for (int k = 0; k < node->array_size; k++) {
            printf("%d ", node->data[k]);
        }
        printf("\n");
    }
}

// free list
void freeList(List *list) {
    Node *to_delete = list->head->next, *next;
    while (to_delete != list->tail) {
        next = to_delete->next;
        remove_node(to_delete);
        to_delete = next;
    }
}

// read int vector
void read_vector(int tab[], size_t n) {
    for (size_t i = 0; i < n; ++i) {
        scanf("%d", tab + i);
    }
}

// initialize the list and push data
void read_list(List *list) {
    int n;
    size_t size;
    scanf("%d", &n); // number of nodes
    for (int i = 0; i < n; i++) {
        scanf("%zu", &size); // length of the array in i-th node
        int *tab = (int *) safe_malloc(size * sizeof(int));
        read_vector(tab, size);
        push_back(list, tab, size);
    }
}

int main() {
    setbuf(stdout, 0);
    int to_do, value;
    size_t size, m;
    List list;
    init(&list);

    scanf("%d", &to_do);
    switch (to_do) {
        case 1:
            read_list(&list);
            dumpList(&list);
            break;
        case 2:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_forward(&list, m));
            }
            printf("\n");
            break;
        case 3:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                printf("%d ", get_backward(&list, m));
            }
            printf("\n");
            break;
        case 4:
            read_list(&list);
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%zu", &m);
                remove_at(&list, m);
//                dumpList(&list);
            }
            dumpList(&list);
            break;
        case 5:
            scanf("%zu", &size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &value);
                put_in_order(&list, value);
            }
            dumpList(&list);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    freeList(&list);

    return 0;
}