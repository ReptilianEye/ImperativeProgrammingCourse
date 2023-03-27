//
// Created by piotr on 27.03.2023.
//
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#define TEST 1 // 1 dla testowania, 0 dla automatycznej oceny

int count[MAX_DIGRAMS] = {0};

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp(const void *a, const void *b)
{
    int va = *(int *)a;
    int vb = *(int *)b;
    if (count[va] == count[vb])
        return va - vb; // sort alphabetically if counts equal
    return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di(const void *a, const void *b)
{
    int va = *(int *)a;
    int vb = *(int *)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS)
        return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb])
        return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int *nl, int *nw, int *nc, FILE *stream)
{

    char line[128];
    *nl = 0;
    *nw = 0;
    *nc = 0;
    while (fgets(line, sizeof(line), stream) != NULL)
    {
        for (int i = 0; line[i] != '\n'; i++)
        {
            //            if (line[i] != ' ')
            *nc = *nc + 1;
            if (line[i] == ' ' && line[i + 1] != ' ')
            {
                *nw = *nw + 1;
            }
        }
        //        *nw = *nw + 1;
        *nl = *nl + 1;
    }
    *nw = *nw + *nl - 2;
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt, FILE *stream)
{
    int zlicz[MAX_CHARS];
    for (int i = 0; i < MAX_CHARS; i++)
        zlicz[i] = 0;
    char line[128];
    int i = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        i = 0;
        while (line[i] != '\n')
        {
            zlicz[line[i] - FIRST_CHAR]++;
            i++;
        }
    }
    int poz_max = -1;
    i = 1;
    while (1)
    {
        int _max = -1;
        for (int j = 0; j < MAX_CHARS; j++)
            if (_max < zlicz[j])
            {
                _max = zlicz[j];
                poz_max = j;
            }
        if (i == char_no)
        {
            *n_char = (char)(poz_max + FIRST_CHAR);
            *cnt = zlicz[poz_max];
            return;
        }
        i++;
        zlicz[poz_max] = -1;
    }
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE *stream)
{
    int zlicz[MAX_CHARS][MAX_CHARS];
    int n = MAX_CHARS;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            zlicz[i][j] = 0;

    char line[128];
    int i = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        i = 0;
        while (line[i + 1] != '\n')
        {
            zlicz[line[i + 1] - FIRST_CHAR][line[i] - FIRST_CHAR]++;
            i++;
        }
    }
    i = 1;
    while (1)
    {
        int max_v = -1;
        int first = -1;
        int second = -1;
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
            {
                if (max_v < zlicz[j][k])
                {
                    max_v = zlicz[j][k];
                    first = j;
                    second = k;
                }
            }
        if (i == digram_no)
        {
            digram[0] = second + FIRST_CHAR;
            digram[1] = first + FIRST_CHAR;
            digram[2] = zlicz[first][second];
            return;
        }
        max_v = -1;
        zlicz[first][second] = 0;
        i++;
    }
}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter, FILE *stream)
{
    *line_comment_counter = 0;
    *block_comment_counter = 0;

    char line[128];
    int i = 0;
    int czy_blokowy = 0;
    while (fgets(line, sizeof(line), stdin) != NULL)
    {
        i = 0;
        while (line[i + 1] != '\n')
        {
            if (line[i] == '*' && line[i + 1] == '/' && czy_blokowy)
            {
                czy_blokowy = 0;
            }
            else if (!czy_blokowy)
            {
                if (line[i] == '/' && line[i + 1] == '*')
                {
                    czy_blokowy = 1;
                    *block_comment_counter = *block_comment_counter + 1;
                }
                else if (line[i] == '/' && line[i + 1] == '/')
                {
                    *line_comment_counter = *line_comment_counter + 1;
                    break;
                }
            }
            i++;
        }
    }
}


#define MAX_LINE 128

int read_line()
{
    char line[MAX_LINE];
    int n;

    fgets(line, MAX_LINE, stdin); // to get the whole line
    sscanf(line, "%d", &n);
    return n;
}

int main(void)
{
    setbuf(stdout, 0);

    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    char file_name[40];
    FILE *stream;

    if (TEST)
        printf("Wpisz nr zadania ");
    to_do = read_line(); //    scanf ("%d", &to_do);
    if (TEST)
        stream = stdin;
    else
    {
        scanf("%s", file_name);
        read_line();
        stream = fopen(file_name, "r");
        if (stream == NULL)
        {
            printf("fopen failed\n");
            return -1;
        }
    }

    //	to_do = read_line();
    switch (to_do)
    {
    case 1: // wc()
        wc(&nl, &nw, &nc, stream);
        printf("%d %d %d\n", nl, nw, nc);
        break;
    case 2: // char_count()
        if (TEST)
            printf("Wpisz numer znaku ");
        char_no = read_line(); //    scanf("%d",&char_no);
        char_count(char_no, &n_char, &cnt, stream);
        printf("%c %d\n", n_char, cnt);
        break;
    case 3: // digram_count()
        if (TEST)
            printf("Wpisz numer digramu ");
        char_no = read_line(); //  scanf("%d",&char_no);
        digram_count(char_no, digram, stream);
        printf("%c%c %d\n", digram[0], digram[1], digram[2]);
        break;
    case 4:
        find_comments(&line_comment_counter, &block_comment_counter, stream);
        printf("%d %d\n", block_comment_counter, line_comment_counter);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}
