//
// Created by piotr on 16.03.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

int card_types = 13;
int card_colors = 4;
#define max_cards 52
#define TEST 1

typedef struct Player {
    int qsize, out, len, empty, in;
    int hand[max_cards];
    int table[max_cards]
} rbuffer;

struct Player init(int max_n);

double hand_push(rbuffer *player, int cli_nr);

int hand_pop(rbuffer *player);

int hand_state(rbuffer *player);

void hand_print(rbuffer *player);

double table_push(rbuffer *player, int curr);

int table_pop(rbuffer *player);

int table_state(rbuffer *player);

void table_print(rbuffer *player);

void swap(int *a, int *b);

int rand_from_interval(int a, int b);

void rand_permutation(int n, int tab[]);

void split_cards(rbuffer *p1, rbuffer *p2);

int define_winner(int p1, int p2); //return 0 if draw, 1 if first win, 2 if second

int simul_game(rbuffer *p1, rbuffer *p2, int easy, int max_conflicts);

int put_card_on_table(rbuffer *p);

void assign_cards(rbuffer *winner, rbuffer *loser);


int main(void) {
    int seed, easy, max_confilcts, game_result;
//    if (TEST)
//        printf("Podaj seed, wersja (0 trudna) i ilosc mozliwych konfiltow");
//    scanf("%d %d %d", &seed, &easy, &max_confilcts);
    seed = 10444;
    easy = 0;
    max_confilcts = 100;
    srand(seed);

    //    const int max_cards = card_colors * card_types;
    setbuf(stdout, 0);
    rbuffer player1 = init(max_cards);
    rbuffer player2 = init(max_cards);
    split_cards(&player1, &player2);

    game_result = simul_game(&player1, &player2, easy, max_confilcts);
    if (game_result == 0) {
        printf("%d\n", game_result);
        printf("%d\n", hand_state(&player1));
        printf("%d\n", hand_state(&player2));
        return 0;
    }
    if (game_result == 1) {
        printf("%d\n", game_result);
        printf("%d\n", hand_state(&player1));
        printf("%d\n", hand_state(&player2));
        return 0;
    }
    if (game_result == 2)
        return 0;
    if (game_result == 3) {
        printf("%d\n", game_result);
        hand_print(&player2);
        return 0;
    }
    printf("Blaaad zly wynik gry");
    return -1;
}


int simul_game(rbuffer *p1, rbuffer *p2, int easy, int max_conflicts) {
    int p1_card, p2_card, res, conflicts, currently_draw;
    currently_draw = 0;
    conflicts = 0;
    while (conflicts < max_conflicts) {
        if (hand_state(p1) == 0 || hand_state(p2) == 0)
            break;
        p1_card = put_card_on_table(p1);
        p2_card = put_card_on_table(p2);
        res = define_winner(p1_card, p2_card);
        if (res == 0) {
            {
                if (!easy) {
                    if (hand_state(p1) == 0 || hand_state(p2) == 0)
                        break;
                    p1_card = put_card_on_table(p1); //hidded card on table if draw
                    p2_card = put_card_on_table(p2);
                    conflicts--; // to balance increment later
                    currently_draw = 1;
                } else {
                    hand_push(p1, table_pop(p1));
                    hand_push(p2, table_pop(p2));
                }
            }
        } else {
            if (res == 1)
                assign_cards(p1, p2);
            else
                assign_cards(p2, p1);
            currently_draw = 0;
        }
        conflicts++;
    }
    if (currently_draw) {
        assign_cards(p1, p1);
        assign_cards(p2, p2);
        return 1;
    }
    if (conflicts == max_conflicts) {
        assign_cards(p1, p1);
        assign_cards(p2, p2);
        return 0;
    }
    if (hand_state(p2) == 0) {
        printf("%d\n", 2);
        printf("%d\n", conflicts);
        return 2;
    }
    if (hand_state(p1) == 0)
        return 3;
    return -1000; //if error, should never happen

}

struct Player init(int n) {
    rbuffer player;
    int empty = -1; // fill with the player if empty


    player.qsize = n;
    player.len = 0;
    player.out = 0;
    player.empty = empty;
    player.in = 0;

    for (int i = 0; i < n; i++) {
        player.hand[i] = empty;
        player.table[i] = empty;
    }
    return player;
}

void assign_cards(rbuffer *winner, rbuffer *loser) {
    int card;
    while (1) {
        card = table_pop(winner);
        if (card == -1)
            break;
        hand_push(winner, card);
    }
    while (1) {
        card = table_pop(loser);
        if (card == -1)
            break;
        hand_push(winner, card);
    }
}

int put_card_on_table(rbuffer *p) {
    int card = hand_pop(p);
    table_push(p, card);
    return card;
}


void split_cards(rbuffer *p1, rbuffer *p2) {
    int n = p1->qsize;
    int cards[n];
    rand_permutation(n, cards);
    for (int i = 0; i < n; i++)
        if (i < n / 2)
            hand_push(p1, cards[i]);
        else
            hand_push(p2, cards[i]);
}

int define_winner(int p1, int p2) {
    int score1 = floor(p1 / card_colors);
    int score2 = floor(p2 / card_colors);
    if (score1 == score2)
        return 0;
    if (score1 > score2)
        return 1;
    else
        return 2;
}


double hand_push(rbuffer *player, int cli_nr) {
    if (player->len == player->out && player->hand[player->len] != player->empty)
        return INFINITY;
    player->hand[player->len] = cli_nr;
    player->len = (player->len + 1) % player->qsize;
}

int hand_pop(rbuffer *player) {
    if (player->out == player->len && player->hand[player->len] == player->empty)
        return player->empty;
    else {
        int c_cout = player->hand[player->out];
        player->hand[player->out] = player->empty;
        player->out = (player->out + 1) % player->qsize;
        return c_cout;
    }
}

int hand_state(rbuffer *player) {
    if (player->out < player->len)
        return player->len - player->out;
    if (player->hand[player->out] == player->empty)
        return 0;
    if (player->out == player->len && player->hand[player->len] != player->empty)
        return player->qsize;
    return player->qsize - (player->out - player->len);

}

void hand_print(rbuffer *player) {
    int i = player->out;
    if (player->hand[player->out] != player->empty) {
        do {
            printf("%d ", player->hand[i]);
            i = (i + 1) % player->qsize;
        } while (i != player->len);
        printf("\n");
    }
}


double table_push(rbuffer *player, int x) {
    if (player->in < player->qsize) {
        player->table[player->in] = x;
        player->in++;
    } else
        printf("ERRRRRRRRROR stol przepelniony");
    return 0;
}


int table_pop(rbuffer *player) {
    int out = player->table[0];
    if (out == -1) {
//        printf("ERRRRRRRRRR za malo elementow w kolejce");
        return -1;
    }
    int i;
    for (i = 0; i < player->in; i++)
        player->table[i] = player->table[i + 1];
    player->table[i] = player->empty;
    player->in--;
    return out;
}

int table_state(rbuffer *player) {
    return player->in;
}

void table_print(rbuffer *player) {
    for (int i = 0; i < player->in; i++) {
        printf("%d ", player->table[i]);
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;

}

int rand_from_interval(int a, int b) {
    if (a > b) return INT_MIN;
    if (b - a > RAND_MAX) return INT_MAX;
    if (a == b) return a;
    return a + rand() % (b - a + 1);
}

// Losowa permutacja elementow zbioru liczb {0, 1, 2,..., n-1} (z rozkladem rownomiernym)
// wg algorytmu przedstawionego w opisie zadania
// jest zapisywana w tablicy tab.
// 0 < n <= 100, jezeli nie to elementy tablicy tab maja wartosci nieokreslone.
void rand_permutation(int n, int tab[]) {
    int i, rand_poz;
    for (i = 0; i < n; i++)
        tab[i] = i;
    for (i = 0; i < n - 1; i++) {
        rand_poz = rand_from_interval(i, n - 1);
        swap(&tab[i], &tab[rand_poz]);
    }
}