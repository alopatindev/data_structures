#ifndef BINTREE_H
#define BINTREE_H

#include <stdlib.h>

#define HAS_LEFT(node) ((node)->left != NULL)
#define HAS_RIGHT(node) ((node)->right != NULL)
#define IS_LEAF(node) (!HAS_LEFT(node) && !HAS_RIGHT(node))

#define TABWIDTH 4

typedef struct StructNode
{
    struct StructNode *left, *right, *parent;
    int data;
} Node;

typedef struct
{
    Node *root;
    size_t size;
} BinTree;

BinTree *createBinTree();
void printBinTree(BinTree *tree);
void printBinTreeByLevel(BinTree *tree);
void insertBinNode(int data, BinTree *tree);
Node *search(Node *root, int data);
void removeBinNode(Node **node, BinTree *tree);
void removeBinNodeByData(int data, BinTree *tree);
void clearBinTree(BinTree *tree);
void freeBinTree(BinTree **tree);
int isEmptyBinTree(BinTree *tree);

void traversalPreorder(Node *root, void (*f)(Node *, int), int level);
void traversalInorder(Node *root, void (*f)(Node *, int), int level);
void traversalPostorder(Node *root, void (*f)(Node *, int), int level);
void traversalLevelorder(Node *root, void (*f)(Node *, int));

Node *createBinNode(int data, Node *parent, Node *left, Node *right);
void insert1(int data, Node **node, Node *parent);
Node *findMin(Node *node);
void updateParent(Node **node, Node *value);
void printBinNode(Node *node, int level);
void freeBinNode(Node *node, int level);

#endif
