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

struct PKeyNode
{
    char key[MAXKEYSIZE];
    struct HNode *node;
};

struct Heap *createHeap();
struct HNode *createHNode(char *key, HDATATYPE *data, struct HNode *parent,
                          struct HNode *left, struct HNode *right);
void insertHNode(struct Heap *heap, char *key, HDATATYPE *data);
struct HNode *searchHeap(struct Heap *heap, char *key);
void printHeap(struct Heap *heap);
struct HNode *searchHeap(struct Heap *heap, char *key);
void clearHeap(struct Heap *heap);
void freeHeap(struct Heap **heap);

void traversalPreorder(struct HNode *root,
                       int (*f)(struct HNode *, int, void *), void *adata);
void traversalInorder(struct HNode *root,
                      int (*f)(struct HNode *, int, void *), void *adata);
void traversalPostorder(struct HNode *root,
                        int (*f)(struct HNode *, int, void *), void *adata);
void traversalLevelorder(struct HNode *root,
                         int (*f)(struct HNode *, int, void *), void *adata);
void traversalLevelorder2(struct HNode *root,
                          int (*f)(struct HNode *, int, void *), void *adata);

int compareKeys(char *key1, char *key2);
int equalKeys(char *key1, char *key2);
int notMatchHNode(struct HNode *node, int height, void *adata);
void heapFixup(struct Heap *heap, struct HNode *n);
void swap(struct Heap *heap, struct HNode *h1, struct HNode *h2);
struct HNode *insertHNodeBinary(struct Heap *heap, char *key, HDATATYPE *data);
struct HNode *createHNode(char *key, HDATATYPE *data, struct HNode *parent,
                          struct HNode *left, struct HNode *right);
int printHNode(struct HNode *node, int height, void *adata);
int freeHNode(struct HNode *node, int height, void *adata);

#endif
