//
// Created by piotr on 16.03.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define max_cards 52
#define TEST 1
int card_colors = 4;

typedef struct Player { //object used to represent Player; their hand and table (as objected as it possible)
    int qsize, out, len, empty, in;
    int hand[max_cards];
    int table[max_cards];
} rbuffer;

struct Player init(int max_n); // function returns instance of player
double hand_push(rbuffer *player, int cli_nr);
int hand_pop(rbuffer *player);
int hand_state(rbuffer *player);
void hand_print(rbuffer *player);
double table_push(rbuffer *player, int curr);
int table_pop(rbuffer *player);
void swap(int *a, int *b);
int rand_from_interval(int a, int b); // used to generate permutations - shuffle cards
void rand_permutation(int n, int tab[]); // shuffles cards
void split_cards(rbuffer *p1, rbuffer *p2); //shuffles cards and split them beetween players, inserting them to their hands
int define_winner(int p1, int p2); //return 0 if draw, 1 if first win, 2 if second
int simul_game(rbuffer *p1, rbuffer *p2, int easy, int max_conflicts);
int put_card_on_table(rbuffer *p); //places card on table and return its value
void assign_cards(rbuffer *winner, rbuffer *loser); //assign cards from "table" of both players to the player how won

// In the game each player will use two structures "hand" and "table" in front of a player.
// If player play a card it pops from they hand and pushes to "table" as long as the duel is not decided.
// Then cards which they won go back to his "hand"
int conflicts = 0;

int main(void) {
    int seed, easy, max_confilcts, game_result;
    if (TEST)
        printf("Podaj seed, wersje (0 trudna, 1 uproszczona), i ilosc mozliwych konfiltow przed koncem gry (oddzielone spacjami): ");
    scanf("%d %d %d", &seed, &easy, &max_confilcts);
    srand(seed);

    setbuf(stdout, 0); //used to see console output while debugging
    rbuffer player1 = init(max_cards); //initialize players
    rbuffer player2 = init(max_cards);
    split_cards(&player1, &player2);
    printf("\n");
    hand_print(&player1);
    hand_print(&player2);


    game_result = simul_game(&player1, &player2, easy, max_confilcts); //simulate game
    switch (game_result) {
        case (0):
            printf("%d\n", game_result);
            printf("%d\n", hand_state(&player1));
            printf("%d\n", hand_state(&player2));
            break;
        case (1):
            printf("%d\n", game_result);
            printf("%d\n", hand_state(&player1));
            printf("%d\n", hand_state(&player2));
            break;
        case (2):
            printf("%d\n", 2);
            printf("%d\n", conflicts);
            break;
        case (3):
            printf("%d\n", game_result);
            hand_print(&player2);
            break;
        default:
            printf("ERR: Wrong result"); //should never happen
            return -111;
    }
    return 0;
}

int simul_game(rbuffer *p1, rbuffer *p2, int easy, int max_conflicts) {
    int p1_card, p2_card, res, currently_draw = 0;

    while (conflicts < max_conflicts) {
        if (hand_state(p1) == 0 || hand_state(p2) == 0) //if players have cards in hand
            break;
        p1_card = put_card_on_table(p1); //play cards
        p2_card = put_card_on_table(p2);
        res = define_winner(p1_card, p2_card);
        if (res == 0) {
                if (!easy) {
                    if (hand_state(p1) == 0 || hand_state(p2) == 0)
                        break;
                    put_card_on_table(p1); //hidded card on table if draw (play cards but with no consequences
                    put_card_on_table(p2);
                    conflicts--; // to balance increment later (in normal version draw is not counted as conflict)
                    currently_draw = 1; // to if game ended while deciding draw
                } else {
                    hand_push(p1, table_pop(p1)); // in easy version in case of a draw cards go back to hand of a player
                    hand_push(p2, table_pop(p2));
                }
        } else {
            if (res == 1)
                assign_cards(p1, p2); //1st player won
            else
                assign_cards(p2, p1); //2nd player won
            currently_draw = 0; //draw decided
        }
        conflicts++;
    }
    if (currently_draw) {
        assign_cards(p1, p1); //if at the end of cards in hand of one of the players their table cards go back to their hands
        assign_cards(p2, p2);
        return 1;
    }
    if (conflicts == max_conflicts) { // if limit of conflicts is reached cards on table returns to hands of the players
        assign_cards(p1, p1);
        assign_cards(p2, p2);
        return 0;
    }
    if (hand_state(p2) == 0) { //handling winning of 1st player
        if (!easy)  // for some reason with it works
            conflicts++;
        return 2;
    }
    if (hand_state(p1) == 0)
        return 3;
    return -1000; //if error, should never happen

}

struct Player init(int n) {
    rbuffer player;
    int empty = -1; // fill with if empty

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
        return -1;
    }
    int i;
    for (i = 0; i < player->in; i++)
        player->table[i] = player->table[i + 1];
    player->table[i] = player->empty;
    player->in--;
    return out;
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

void rand_permutation(int n, int tab[]) {
    int i, rand_poz;
    for (i = 0; i < n; i++)
        tab[i] = i;
    for (i = 0; i < n - 1; i++) {
        rand_poz = rand_from_interval(i, n - 1);
        swap(&tab[i], &tab[rand_poz]);
    }
}
