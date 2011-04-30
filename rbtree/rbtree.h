#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>

#define HAS_LEFT(node) ((node)->left != NULL)
#define HAS_RIGHT(node) ((node)->right != NULL)
#define IS_LEAF(node) (!HAS_LEFT(node) && !HAS_RIGHT(node))

#define TABWIDTH 4

typedef char KeyType[20];
typedef char DataType[20];

enum Color {Red, Black};

struct RBNode
{
    struct RBNode *left, *right, *parent;
    KeyType key;
    DataType data;
    enum Color color;
};

struct RBTree
{
    RBNode *root;
    int size;
    int height;
};

STACK(StackRBNodes, RBNode *)
STACK(StackRBLevels, int)

struct RBTree *createRBTree();
void printRBTree(struct RBTree *tree);
void printRBTreeByLevel(struct RBTree *tree);
void insertRBNode(struct RBTree *tree, KeyType key, DataType *data);
struct RBNode *searchRBTree(struct RBTree *tree, KeyType key);
//void removeRBNode(struct RBNode **node, struct RBTree *tree);
//void removeRBNodeByKey(KeyType key, struct RBTree *tree);
void clearRBTree(struct RBTree *tree);
void freeRBTree(struct RBTree **tree);
int isEmptyRBTree(struct RBTree *tree);

// 1 means less, 0 means more, -1 means equal
int compareRBNodes(struct RBNode *one, struct RBNode *two);

void traversalPreorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalInorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalPostorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalLevelorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));

struct RBNode *createRBNode(KeyType key, DataType *data, struct RBNode *parent,
                            struct RBNode *left, struct RBNode *right);
struct RBNode *findMinRBTree(struct RBNode *node);
void printRBNode(struct RBNode *node, int height);
void freeRBNode(struct RBNode *node, int height);

#endif
