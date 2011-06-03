#include <stdio.h>
#include <string.h>
#include "bintree.h"

#define MAX_INPUT_LEN KEY_LENGTH
#define CALLBACK_SIGNATURE struct BinTree *dict, char *in
#define TEMP_FILENAME "/tmp/smalldicty.txt"

struct Literal
{
    char key[MAX_INPUT_LEN];
    int (*function)(CALLBACK_SIGNATURE);
};

void input(char *dest)
{
    int i = 0;
    char c;
    while (i < MAX_INPUT_LEN && (c = getchar()) != '\n')
        dest[i++] = c;
    dest[i] = '\0';
}

static int help(CALLBACK_SIGNATURE)
{
    printf("Short help. Just input a word or one of those commands:\n"
           "  /help - show this text\n"
           "  /about - some words about this program\n"
           "  /add - add a word\n"
           "  /edit - edit a word\n"
           "  /remove - remove a word\n"
           "  /quit - exit this program\n"
           "  /store - save data to disk\n"
           "  /load - load data from disk\n" 
           "  /tree - print a binary tree (for debugging only)\n\n");
    return 1;
}

static int about(CALLBACK_SIGNATURE)
{
    printf(
        "smalldicty is a small dictionary for any language(s).\n"
        "Copyright (C) 2011 Lopatin Alexander\n\n"
        "This program is free software: you can do everything you want with "
        "it. It's licensed under the terms of GPL 3 (please visit "
        "http://www.gnu.org/licenses/gpl-3.0.html for details).\n");
    return 1;
}

static int quit(CALLBACK_SIGNATURE)
{
    return 0;
}

static int query(CALLBACK_SIGNATURE)
{
    struct BinNode *n = search(dict->root, in);
    printf("   %s\n", n ? n->value : "Not found.");
    return 1;
}

static int addWord(CALLBACK_SIGNATURE)
{
    char key[KEY_LENGTH];
    char value[VALUE_LENGTH];

    printf("Enter a word: ");
    input(key);

    if (search(dict->root, key)) {
        printf("This word exists already. Use /edit to edit it.\n");
        return 1;
    }
    system("\"${EDITOR}\" " TEMP_FILENAME);

    FILE *f = fopen(TEMP_FILENAME, "r");
    if (!f)
        return 1;

    (void)fread(value, VALUE_BLOCKS, VALUE_BLOCK_SIZE, f);
    //fscanf(f, "%s", value);

    //printf("Created a word ``%s'' with ``%s' value.\n", key, value);

    insertBinNode(dict, key, value);
    fclose(f);
    (void)remove(TEMP_FILENAME);
    return 1;
}

static int editWord(CALLBACK_SIGNATURE)
{
    char key[KEY_LENGTH];
    char value[VALUE_LENGTH];

    printf("Enter a word you want to edit: ");
    input(key);

    struct BinNode *n = search(dict->root, key);
    if (!n) {
        printf("This word does not exist. Use /add to create it.\n");
        return 1;
    }
    FILE *f = fopen(TEMP_FILENAME, "w");
    fwrite(n->value, strnlen(n->value, VALUE_LENGTH), 1, f);
    fclose(f);

    system("\"${EDITOR}\" " TEMP_FILENAME);

    f = fopen(TEMP_FILENAME, "r");
    if (!f)
        return 1;

    (void)fread(value, VALUE_BLOCKS, VALUE_BLOCK_SIZE, f);
    //fscanf(f, "%s", value);

    //printf("Updated a word ``%s'' with ``%s'' value.\n", key, value);

    memcpy(n->value, value, VALUE_LENGTH);
    fclose(f);
    (void)remove(TEMP_FILENAME);
    return 1;
}

static int removeWord(CALLBACK_SIGNATURE)
{
    char key[KEY_LENGTH];
    printf("Enter a word you wish to remove: ");
    input(key);

    struct BinNode *n = search(dict->root, key);
    if (!n) {
        printf("Not found.\n");
        return 1;
    }
    printf("Removing '%s'...", n->key);
    removeBinNode(dict, &n);
    printf(" done\n");
    return 1;
}

void printTree(struct BinNode *node, int height)
{
    int i;
    for (i = 0; i < height*3; ++i)
        printf(" ");

    if (node) {
        printf("%s\n", node->key);
        printTree(node->left, height + 1);
        printTree(node->right, height + 1);
    } else {
        printf("NULL\n");
    }
}

int printTreeWrapper(CALLBACK_SIGNATURE)
{
    printTree(dict->root, 0);
    return 1;
}

static int store(CALLBACK_SIGNATURE)
{
    traversalToFile(dict->root, &appendToFile);
    return 1;
}

static int load(CALLBACK_SIGNATURE)
{
    FILE *f = fopen(DICT_FILE, "r");
    struct DictPair p;
    while (!feof(f)) {
        fread(&p, sizeof(struct DictPair), 1, f);
        insertBinNode(dict, p.key, p.value);
    }
    fclose(f);
    return 1;
}

static const struct Literal literals[] = {
    { .key = "/quit", .function = &quit, },
    { .key = "/help", .function = &help, },
    { .key = "/about", .function = &about, },
    { .key = "/add", .function = &addWord, },
    { .key = "/edit", .function = &editWord, },
    { .key = "/remove", .function = &removeWord, },
    { .key = "/store", .function = &store, },
    { .key = "/load", .function = &load, },
    { .key = "/tree", .function = &printTreeWrapper, },
    //{ .key = "", .function = &query, },
};

int main()
{
    struct BinTree *dict = createBinTree();
    char in[MAX_INPUT_LEN];

    printf("\x5b\x1b[H\x0b\x1b[J");  // clear screen
    printf("This is a small dictionary. Type ``/help'' to get details.\n");

    int litsNumber = sizeof(literals)/sizeof(struct Literal);
    int i, running;
    for (;;) {
        printf("\nsmalldicty >> ");
        input(in);

        i = 0;
        if (in[0] == '/') {
            while (i < litsNumber) {
                if (strncmp(in, literals[i].key, MAX_INPUT_LEN) == 0) {
                    running = literals[i].function(dict, in);
                    break;
                }
                ++i;
            }
            if (i == litsNumber) {
                printf("No such command. Type ``/help'' to get help.\n");
                continue;
            }
            if (!running)
                break;
        } else {
            (void)query(dict, in);
        }
    }
    printf("Good bye!\n");
    freeBinTree(&dict);
    return 0;
}
