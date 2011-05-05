#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#include "stack.h"

STACK(StackRBLevels, int)
STACK(StackRBNodes, struct RBNode *)

int compare(char *s1, char *s2) // operator<
{
//    if (strnlen(s1, MAXKEYSIZE) > strnlen(s2, MAXKEYSIZE))
//        return 0;
    return strncmp(s1, s2, MAXKEYSIZE) <= 0;
}

int equalStrings(char *s1, char *s2, size_t n)
{
    size_t i;
    for (i = 0; i < n; ++i)
        if (s1[i] != s2[i])
            return 0;

    return 1;
}

struct RBTree *createRBTree()
{
    struct RBTree *tree = (struct RBTree *)malloc(sizeof(struct RBTree));
    if (!tree) {
        fprintf(stderr, "Failed to allocate memory for the red-black tree\n");
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    tree->height = -1;

    return tree;
}

struct RBNode *createRBNode(char *key, RBDATATYPE *data, struct RBNode *parent,
                            struct RBNode *left, struct RBNode *right)
{
    struct RBNode *node = (struct RBNode *)malloc(sizeof(struct RBNode));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for the red-black node\n");
        return NULL;
    }

    //*(node->key) = &(key[0]);
    strncpy(node->key, key, MAXKEYSIZE);
    //node->data = *data;
    memcpy(&(node->data), data, sizeof(RBDATATYPE));
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

/*void insert1(char *key, RBDATATYPE *data, struct RBNode **node, struct RBNode *parent)
{
    if (!node)
        return;

    if (!(*node)) {
        *node = createRBNode(key, data, parent, NULL, NULL);
        return;
    }

    //if (*data < (*node)->data)
    if (compare(key, (*node)->key))
        insert1(key, data, &((*node)->left), *node);
    else
        insert1(key, data, &((*node)->right), *node);
}

void insertRBNodeRecursive(char *key, RBDATATYPE *data, struct RBTree *tree)
{
    struct RBNode **root = &(tree->root);

    if (!root)
        return;

    if (!(*root)) {
        *root = createRBNode(key, data, NULL, NULL, NULL);
        if (!(*root))
            return;
    } else {
        insert1(key, data, root, (*root)->parent);
    }

    tree->size++;
}*/

void insertRBNode(struct RBTree *tree, char *key, RBDATATYPE *data)
{
    if (!tree->root) {
        tree->root = createRBNode(key, data, NULL, NULL, NULL);
        return;
    }

    struct RBNode *node = tree->root, *parent = NULL;

    int height;
    for (height = 0; node; ++height) {
        if (equalStrings(key, node->key, MAXKEYSIZE)) {
            return;
        } else {
            parent = node;
            node = compare(key, node->key) ? node->left : node->right;
        }
    }

    if (height > tree->height)
        tree->height++;

    struct RBNode *n = createRBNode(key, data, NULL, NULL, NULL);
    if (!n)
        return;
    
    if (compare(key, parent->key))
        parent->left = n;
    else
        parent->right = n;

    tree->size++;
}

void traversalPreorder(struct RBNode *root, void (*f)(struct RBNode *, int))
{
    struct StackRBNodes *nodes = createStackRBNodes();
    struct StackRBLevels *levels = createStackRBLevels();
    struct RBNode *node = root;
    int height = 0;
    pushStackRBNodes(nodes, &node);
    pushStackRBLevels(levels, &height);
    while (!isEmptyStackRBNodes(nodes)) {
        struct LISTStackRBNodes *n = popStackRBNodes(nodes);
        struct LISTStackRBLevels *h = popStackRBLevels(levels);
        node = n->data;
        height = h->data;
        free(n); free(h);

        f(node, height);

        if (node->right) {
            int h = height + 1;
            pushStackRBNodes(nodes, &(node->right));
            pushStackRBLevels(levels, &h);
        } else {
            f(NULL, height + 1);
        }
        if (node->left) {
            int h = height + 1;
            pushStackRBNodes(nodes, &(node->left));
            pushStackRBLevels(levels, &h);
        } else {
            f(NULL, height + 1);
        }
    }
    freeStackRBNodes(&nodes);
    freeStackRBLevels(&levels);
}

void traversalPostorderRecursive(struct RBNode *root,
                                 void (*f)(struct RBNode *, int), int height)
{
    if (root)
        traversalPostorderRecursive(root->left, f, height + 1);
    
    if (root)
        traversalPostorderRecursive(root->right, f, height + 1);

    f(root, height);
}

void traversalPostorder(struct RBNode *root, void (*f)(struct RBNode *, int))
{
    struct StackRBNodes *nodes = createStackRBNodes();
    struct StackRBLevels *levels = createStackRBLevels();
    struct RBNode *node = root;
    int height = 0;

    while (node || !isEmptyStackRBNodes(nodes)) {
        if (!node) {
            f(NULL, height);
            if (!(topStackRBNodes(nodes)->data->right))
                f(NULL, height);

            while (!isEmptyStackRBNodes(nodes) &&
                   topStackRBNodes(nodes)->data->right == node)
            {
                struct LISTStackRBNodes *n = popStackRBNodes(nodes);
                struct LISTStackRBLevels *l = popStackRBLevels(levels);
                node = n->data;
                height = l->data;
                free(n); free(l);
                f(node, height);
            }
            node = isEmptyStackRBNodes(nodes)
                   ? NULL : topStackRBNodes(nodes)->data->right;
        }
        if (node) {
            ++height;
            pushStackRBNodes(nodes, &node);
            int h = height-1;
            pushStackRBLevels(levels, &h);
            node = node->left;
        }
    }
    freeStackRBNodes(&nodes);
    freeStackRBLevels(&levels);
}

void freeRBNode(struct RBNode *node, int height)
{
    if (node)
        free((void *)node);
}

void clearRBTree(struct RBTree *tree)
{
    if (!tree || !tree->root || !tree->size)
        return;

    traversalPostorder(tree->root, &freeRBNode);
    tree->size = 0;
}

void freeRBTree(struct RBTree **tree)
{
    clearRBTree(*tree);
    free((void *)*tree);
    tree = NULL;
}

void printRBNode(struct RBNode *node, int height)
{
    int i;
    for (i = height * TABWIDTH; i > 0; --i)
        printf(" ");

    if (node == NULL)
        printf("NULL\n");
    else
        printf(PRINTFORMAT, node->key, node->data);
}
