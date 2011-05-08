#ifndef Heap_H
#define Heap_H

#include <stdlib.h>

#define HAS_LEFT(node) ((node)->left != NULL)
#define HAS_RIGHT(node) ((node)->right != NULL)
#define IS_LEAF(node) (!HAS_LEFT(node) && !HAS_RIGHT(node))

#define MAXKEYSIZE 255
#define HDATATYPE int
#define PRINTFORMAT "'%s' <=> %d\n"
#define TABWIDTH 4

struct HNode
{
    struct HNode *left, *right, *parent;
    char key[MAXKEYSIZE];
    HDATATYPE data;
};

struct Heap
{
    struct HNode *root;
    int size;
    int height;
};

int compareKeys(char *key1, char *key2);
int equalKeys(char *key1, char *key2);
struct Heap *createHeap();
struct HNode *insertHNodeBinary(struct Heap *heap, char *key, HDATATYPE *data);
struct HNode *searchHeap(struct Heap *heap, char *key);
void clearHeap(struct Heap *heap);
void freeHeap(struct Heap **heap);
void traversalPostorder(struct HNode *root, void (*f)(struct HNode *, int));
struct HNode *createHNode(char *key, HDATATYPE *data, struct HNode *parent,
                          struct HNode *left, struct HNode *right);
void printHNode(struct HNode *node, int height);
void freeHNode(struct HNode *node, int height);

#endif
