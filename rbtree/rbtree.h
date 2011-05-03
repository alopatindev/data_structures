#ifndef RBTREE_H
#define RBTREE_H

#include <stdlib.h>

#define HAS_LEFT(node) ((node)->left != NULL)
#define HAS_RIGHT(node) ((node)->right != NULL)
#define IS_LEAF(node) (!HAS_LEFT(node) && !HAS_RIGHT(node))

#define MAXKEYSIZE 255
#define RBDATATYPE int
#define PRINTFORMAT "'%s' <=> %d\n"
#define TABWIDTH 4

enum Color {Red, Black};

struct RBNode
{
    struct RBNode *left, *right, *parent;
    char key[MAXKEYSIZE];
    RBDATATYPE data;
    enum Color color;
};

struct RBTree
{
    struct RBNode *root;
    int size;
    int height;
};

int compare(char *s1, char *s2);
struct RBTree *createRBTree();
//void printRBTree(struct RBTree *tree);
//void printRBTreeByLevel(struct RBTree *tree);
void insertRBNode(struct RBTree *tree, char *key, RBDATATYPE *data);
/*struct RBNode *searchRBTree(struct RBTree *tree, KeyType key);
//void removeRBNode(struct RBNode **node, struct RBTree *tree);
//void removeRBNodeByKey(KeyType key, struct RBTree *tree);*/
void clearRBTree(struct RBTree *tree);
void freeRBTree(struct RBTree **tree);
/*int isEmptyRBTree(struct RBTree *tree);

// 1 means less, 0 means more, -1 means equal
int compareRBNodes(struct RBNode *one, struct RBNode *two);

void traversalPreorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalInorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalPostorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
void traversalLevelorderRBTree(struct RBNode *root, void (*f)(struct RBNode *, int));
*/
void traversalPostorder(struct RBNode *root, void (*f)(struct RBNode *, int));
struct RBNode *createRBNode(char *key, RBDATATYPE *data, struct RBNode *parent,
                            struct RBNode *left, struct RBNode *right);
/*struct RBNode *findMinRBTree(struct RBNode *node);*/
void printRBNode(struct RBNode *node, int height);
void freeRBNode(struct RBNode *node, int height);

#endif
