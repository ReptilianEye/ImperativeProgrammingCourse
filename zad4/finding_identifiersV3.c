#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define TEST 1 // 1 - dla testowania,  0 - dla automatycznej oceny

int index_cmp(const void *, const void *);

int cmp(const void *, const void *);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
        "auto", "break", "case", "char",
        "const", "continue", "default", "do",
        "double", "else", "enum", "extern",
        "float", "for", "goto", "if",
        "int", "long", "register", "return",
        "short", "signed", "sizeof", "static",
        "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"};

int kw_len = 32;

int if_keyword(char *word) {
    for (int i = 0; i < kw_len; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int insert_to_tab(char *word, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (strcmp(word, tab[i]) == 0)
            return 0;
    }
    strcpy(tab[i], word);
    len++;
    return 1;
}

int find_idents(FILE *stream) {
    int len = 0;
    char prev = '\n', c;
    int res;
    int is_block = 0;
    int is_string = 0;
//    char *word = (char *) malloc(MAX_ID_LEN * sizeof(char));

    while ((c = getc(stream)) != EOF) {
        if (!is_block && !is_string) {
            if (prev == '/' && c == '*') {  //block comment
                is_block = 1;
            } else if (prev == '/' && c == '/') { //one line comment
                while (c != '\n') {
                    c = getc(stream);
                }
            } else if (c == '"') { //handling string
                is_string = 1;
            } else if (isblank(prev) || prev == '\n') { //reading word
                char word[MAX_ID_LEN] = {};
                for (int i = 0; c == '_' || isalpha(c) || (i > 0 && isdigit(c)); c = getc(stream), i++) {
                    word[i] = c;
                }
//                strcpy(word, word);
                if (strlen(word) > 0) {
                    res = if_keyword(word);
                    if (!res) {
                        res = insert_to_tab(word, len);
                        if (res) {
                            len++;
                        }
                    }
                }
            }
        } else if (c == '"') {
            is_string = 0;
        } else if (prev == '*' && c == '/') {
            is_block = 0;
        }
        prev = c;
    }
    for (int i = 0; i < len; i++)
        printf("%s\n", tab[i]);
    return len;
}

int cmp(const void *first_arg, const void *second_arg) {
    char *a = *(char **) first_arg;
    char *b = *(char **) second_arg;
    return strcmp(a, b);
}

int index_cmp(const void *first_arg, const void *second_arg) {
    int a = *(int *) first_arg;
    int b = *(int *) second_arg;
    return strcmp(tab[a], tab[b]);
}

int main(void) {
    setbuf(stdout, 0);

    char file_name[40];
    FILE *stream;

    if (TEST)
        stream = stdin;
    else {
        scanf("%s", file_name);
        stream = fopen(file_name, "r");
        if (stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }
    printf("%d\n", find_idents(stream));
    return 0;
}
