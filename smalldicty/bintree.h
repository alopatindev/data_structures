#ifndef BINTREE_H
#define BINTREE_H

#include <stdlib.h>
#include "stack.h"

#define KEY_LENGTH 255
#define VALUE_BLOCK_SIZE 1024
#define VALUE_BLOCKS 32
#define VALUE_LENGTH VALUE_BLOCK_SIZE*VALUE_BLOCKS
#define HAS_LEFT(node) ((node)->left != NULL)
#define HAS_RIGHT(node) ((node)->right != NULL)
#define IS_LEAF(node) (!HAS_LEFT(node) && !HAS_RIGHT(node))
#define IS_ROOT(node) (!((node)->parent))

struct BinNode
{
    struct BinNode *left, *right, *parent;
    char key[KEY_LENGTH];
    char value[VALUE_LENGTH];
};

struct BinTree
{
    struct BinNode *root;
    int height;
    int size;
};

struct BinTree *createBinTree();
struct BinNode *insertBinNode(struct BinTree *tree, char *key, char *value);
struct BinNode *search(struct BinNode *root, char *key);
void removeBinNode(struct BinTree *tree, struct BinNode **node);
void freeBinTree(struct BinTree **tree);
int isEmptyBinTree(struct BinTree *tree);

struct BinNode *createBinNode(char *key,
                              char *value,
                              struct BinNode *parent,
                              struct BinNode *left,
                              struct BinNode *right);
struct BinNode *findMin(struct BinNode *node);
void updateParent(struct BinTree *tree, struct BinNode **node,
                  struct BinNode *value);
void freeBinNode(struct BinNode *node);
int equalKeys(char *s1, char *s2);
int compareKeys(char *s1, char *s2);

void traversalPreorder(struct BinNode *root, void (*f)(struct BinNode *));
void traversalInorder(struct BinNode *root, void (*f)(struct BinNode *));
void traversalPostorder(struct BinNode *root, void (*f)(struct BinNode *));

#endif
