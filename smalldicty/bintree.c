#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bintree.h"

struct BinTree *createBinTree()
{
    struct BinTree *tree = (struct BinTree *)malloc(sizeof(struct BinTree));
    if (!tree) {
        fprintf(stderr, "Failed to allocate memory for a binary tree\n");
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    tree->height = -1;

    return tree;
}

int compareKeys(char *s1, char *s2) // operator<
{
    size_t l1 = strnlen(s1, KEY_LENGTH),
           l2 = strnlen(s2, KEY_LENGTH);
    if (l1 > l2)
        return 0;
    else if (l1 < l2)
        return 1;
    else
        return strncmp(s1, s2, KEY_LENGTH) < 0;
}

int equalKeys(char *s1, char *s2)
{
    size_t i;
    for (i = 0; i < KEY_LENGTH && !(s1[i] == s2[i] && s1[i] == '\0'); ++i)
        if (s1[i] != s2[i])
            return 0;
    return 1;
}

struct BinNode *createBinNode(char *key,
                              char *value,
                              struct BinNode *parent,
                              struct BinNode *left,
                              struct BinNode *right)
{
    struct BinNode *node = (struct BinNode *)malloc(sizeof(struct BinNode));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for a binary node\n");
        return NULL;
    }

    memcpy(node->key, key, KEY_LENGTH);
    memcpy(node->value, value, VALUE_LENGTH);
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

struct BinNode *insertBinNode(struct BinTree *tree, char *key, char *value)
{
    if (!tree->root) {
        tree->root = createBinNode(key, value, NULL, NULL, NULL);
        return tree->root;
    }

    struct BinNode *node = tree->root, *parent = NULL;

    int height;
    for (height = 0; node; ++height) {
        if (equalKeys(key, node->key)) {
            return NULL;
        } else {
            parent = node;
            node = compareKeys(key, node->key)
                ? node->left : node->right;
        }
    }

    if (height > tree->height)
        tree->height++;

    struct BinNode *n = createBinNode(key, value, parent, NULL, NULL);
    if (!n)
        return NULL;
    
    if (compareKeys(key, parent->key))
        parent->left = n;
    else
        parent->right = n;

    tree->size++;
    return n;
}

struct BinNode *search(struct BinNode *root, char *key)
{
    while (root && !equalKeys(root->key, key))
        root = (compareKeys(key, root->key)) ? root->left : root->right;
    return root;
}

struct BinNode *findMin(struct BinNode *node)
{
    if (!node)
        return NULL;

    while (HAS_LEFT(node))
        node = node->left;
    return node;
}

// updates parent's pointer before removal
void updateParent(struct BinTree *tree, struct BinNode **node,
                  struct BinNode *value)
{
    if (!node || !*node || !tree)
        return;

    //if (IS_ROOT(*node)) {
    if (*node == tree->root) {
        tree->root = value;
    } else {
        if ((*node)->parent->left == *node)
            (*node)->parent->left = value;
        else
            (*node)->parent->right = value;

        if (value)
            value->parent = (*node)->parent;
    }
}

void removeBinNode(struct BinTree *tree, struct BinNode **node)
{
    if (!node || !*node)
        return;

    // we're gonna work with original pointer to pointer if it's not
    if ((*node)->parent) {
        node = ((*node)->parent->left == *node)
               ? &((*node)->parent->left)
               : &((*node)->parent->right);
    }

    if (IS_LEAF(*node)) {
        // 1. Deleting a leaf (node with no children):
        // Deleting a leaf is easy, as we can simply remove it from the tree.
        struct BinNode *d = *node;
        updateParent(tree, node, NULL);
        freeBinNode(d);
    } else if (HAS_LEFT(*node) && !HAS_RIGHT(*node)) {
        // 2. Deleting a node with one child:
        // Remove the node and replace it with its child.
        struct BinNode *d = *node;
        updateParent(tree, node, (*node)->left);
        freeBinNode(d);
    } else if (!HAS_LEFT(*node) && HAS_RIGHT(*node)) {
        struct BinNode *d = *node;
        updateParent(tree, node, (*node)->right);
        freeBinNode(d);
    } else {  //  HAS_LEFT(*node) && HAS_RIGHT(*node)
        // 3. Deleting a node with two children:
        // Do not delete "node". Find the smallest (the leftest) child of
        // the right node's subtree (call him "m") and copy from "m" to "node"
        // all data (not including left, right or parent pointers) to "node".
        // Remove m.
        struct BinNode *m = findMin((*node)->right);
        //(*node) = m;
        memcpy((*node)->key, m->key, KEY_LENGTH);
        memcpy((*node)->value, m->value, VALUE_LENGTH);
        removeBinNode(tree, &m);
    }

    tree->size--;
}

void freeBinNode(struct BinNode *node)
{
    if (node)
        free((void *)node);
}

int isEmptyTree(struct BinTree *tree)
{
    return tree->size == 0;
}

void freeBinTree(struct BinTree **tree)
{
    while ((*tree)->root)
        removeBinNode(*tree, &((*tree)->root));
    //traversalPostorder((*tree)->root, freeBinNode);
    free((void *)*tree);
    *tree = NULL;
}

void traversalPreorder(struct BinNode *root, void (*f)(struct BinNode *))
{
    struct Stack *s = createStack();
    struct BinNode *node = root;
    pushStack(s, node);
    while (!isEmptyStack(s)) {
        node = popStack(s);
        f(node);
        if (!node)
            continue;
        if (node->right)
            pushStack(s, node->right);
        else
            f(NULL);
        if (node->left)
            pushStack(s, node->left);
        else
            f(NULL);
    }
    freeStack(&s);
}

void appendToFile(struct BinNode *node, FILE *f)
{
    if (!node)
        return;
    struct DictPair p;
    memcpy(p.key, node->key, KEY_LENGTH);
    memcpy(p.value, node->value, VALUE_LENGTH);
    fwrite((void *)&p, sizeof(struct DictPair), 1, f);
}

void traversalToFile(struct BinNode *root, void (*func)\
                                           (struct BinNode *, FILE *f))
{
    FILE *f = fopen(DICT_FILE, "wa");
    struct Stack *s = createStack();
    struct BinNode *node = root;
    pushStack(s, node);
    while (!isEmptyStack(s)) {
        node = popStack(s);
        func(node, f);
        if (!node)
            continue;
        if (node->right)
            pushStack(s, node->right);
        else
            func(NULL, f);
        if (node->left)
            pushStack(s, node->left);
        else
            func(NULL, f);
    }
    freeStack(&s);
    fclose(f);
}


void traversalInorder(struct BinNode *root, void (*f)(struct BinNode *))
{
    struct Stack *s = createStack();
    struct BinNode *node = root;

    while (!isEmptyStack(s) || node) {
        if (node) {
            pushStack(s, node);
            node = node->left;
        } else {
            f(NULL);
            node = popStack(s);
            f(node);
            node = node->right;
        }
    }
    f(NULL);
    freeStack(&s);
}

void traversalPostorder(struct BinNode *root, void (*f)(struct BinNode *))
{
    struct Stack *s = createStack();
    struct BinNode *node = root;

    while (node || !isEmptyStack(s)) {
        if (!node) {
            f(NULL);
            if (!s->head->node->right)
                f(NULL);
            while (!isEmptyStack(s) && s->head->node->right == node) {
                node = popStack(s);
                f(node);
            }
            node = isEmptyStack(s) ? NULL : s->head->node->right;
        }
        if (node) {
            pushStack(s, node);
            node = node->left;
        }
    }
    freeStack(&s);
}
