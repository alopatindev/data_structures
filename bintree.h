#ifndef BINTREE_H
#define BINTREE_H

#define TABWIDTH 4

typedef struct StructNode
{
    struct StructNode *left, *right, *parent;
    int data;
} Node;

Node *create(int data, Node *parent, Node *left, Node *right);
void insert1(int data, Node **node, Node *parent);
void insert(int data, Node **root);
void print(Node *node, int tab);
void traversalPreorder(Node *root, void (*f)(Node *, int), int level);
void traversalInorder(Node *root, void (*f)(Node *, int), int level);
void traversalPostorder(Node *root, void (*f)(Node *, int), int level);
void traversalLevelorder(Node *root, void (*f)(Node *, int));
Node *search(Node *root, int data);
inline int hasLeft(Node *node);
inline int hasRight(Node *node);
inline int isLeaf(Node *node);
Node *findMin(Node *node);
void updateParent(Node **node, Node *value);
void removeNode(Node **node);
void removeNodeByData(int data, Node *root);
void printNode(Node *node, int level);
void printTree(Node *root);
void freeNode(Node *node, int level);
void freeTree(Node **root);

#endif
