#ifndef BINTREE_H
#define BINTREE_H

#include <stdlib.h>

#define TABWIDTH 4

typedef struct StructNode
{
    struct StructNode *left, *right, *parent;
    int data;
} Node;

typedef struct BinTreeStruct
{
    Node *root;
    size_t size;
} BinTree;

BinTree *createBinTree();
void printTree(BinTree *tree);
void insert(int data, BinTree *tree);
Node *search(Node *root, int data);
void removeNode(Node **node, BinTree *tree);
void removeNodeByData(int data, BinTree *tree);
void clearTree(BinTree *tree);
void freeTree(BinTree **tree);
int isEmptyTree(BinTree *tree);

void traversalPreorder(Node *root, void (*f)(Node *, int), int level);
void traversalInorder(Node *root, void (*f)(Node *, int), int level);
void traversalPostorder(Node *root, void (*f)(Node *, int), int level);
void traversalLevelorder(Node *root, void (*f)(Node *, int));

Node *createNode(int data, Node *parent, Node *left, Node *right);
void insert1(int data, Node **node, Node *parent);
void print(Node *node, int tab);
inline int hasLeft(Node *node);
inline int hasRight(Node *node);
inline int isLeaf(Node *node);
Node *findMin(Node *node);
void updateParent(Node **node, Node *value);
void printNode(Node *node, int level);
void freeNode(Node *node, int level);

#endif
