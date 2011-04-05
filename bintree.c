#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Node *create(int data, Node *parent, Node *left, Node *right)
{
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return NULL;
    }

    node->data = data;
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

void insert1(int data, Node **node, Node *parent)
{
    if (node == NULL)
        return;

    if (*node == NULL) {
        *node = create(data, parent, NULL, NULL);
        return;
    }

    if (data < (*node)->data)
        insert1(data, &((*node)->left), *node);
    else
        insert1(data, &((*node)->right), *node);
}

void insert(int data, Node **root)
{
    if (root == NULL)
        return;

    if (*root == NULL) {
        *root = create(data, NULL, NULL, NULL);
        if (*root == NULL)
            return;
    } else {
        insert1(data, root, (*root)->parent);
    }
}

void print(Node *node, int tab)
{
    int i;
    for (i = 0; i < tab; ++i)
        printf(" ");

    if (node == NULL) {
        printf("NULL\n");
        return;
    }

    printf("%d\n", node->data);
    print(node->left, tab + TABWIDTH);
    print(node->right, tab + TABWIDTH);
}

void traversalPreorder(Node *root, void (*f)(Node *, int), int level)
{
    f(root, level);

    if (root == NULL)
        return;

    traversalPreorder(root->left, f, level + 1);
    traversalPreorder(root->right, f, level + 1);
}

void traversalInorder(Node *root, void (*f)(Node *, int), int level)
{
    if (root != NULL)
        traversalInorder(root->left, f, level);
    
    f(root, level);

    if (root != NULL)
        traversalInorder(root->right, f, level);
}

void traversalPostorder(Node *root, void (*f)(Node *, int), int level)
{
    if (root != NULL)
        traversalInorder(root->left, f, level);
    
    if (root != NULL)
        traversalInorder(root->right, f, level);

    f(root, level);
}

void traversalLevelorder(Node *root, void (*f)(Node *, int))
{
    int level = 0;  // TODO: calculate each node's level
    Queue *q = createQueue();
    Node *n = NULL;
    pushQueue(q, root);
    while (!emptyQueue(q)) {
        n = popQueue(q);
        f(n, level);
        if (n->left != NULL)
            pushQueue(q, n->left);
        if (n->right != NULL)
            pushQueue(q, n->right);
    }
    freeQueue(&q);
}

/*Node *search(Node *root, int data)
{
    if (root == NULL || root->data == data)
        return root;

    if (root->data > data)
        return search(root->left, data);
    else
        return search(root->right, data);
}*/

Node *search(Node *root, int data)
{
    while (root != NULL && root->data != data) {
        if (root->data > data)
            root = root->left;
        else
            root = root->right;
    }
    return root;
}

inline int hasLeft(Node *node) { return node->left != NULL; }
inline int hasRight(Node *node) { return node->right != NULL; }
inline int isLeaf(Node *node) { return !hasLeft(node) && !hasRight(node); }

Node *findMin(Node *node)
{
    if (node == NULL)
        return NULL;

    while (hasLeft(node))
        node = node->left;
    return node;
}

// updates parent's pointer before removal
void updateParent(Node **node, Node *value)
{
    if (node == NULL)
        return;

    if ((*node)->parent == NULL)
        return;

    if ((*node)->parent->left == *node)
        (*node)->parent->left = value;
    else
        (*node)->parent->right = value;

    if (value != NULL)
        value->parent = (*node)->parent;
}

void removeNode(Node **node)
{
    if (node == NULL || *node == NULL)
        return;

    if (isLeaf(*node)) {
        // 1. Deleting a leaf (node with no children):
        // Deleting a leaf is easy, as we can simply remove it from the tree.
        updateParent(node, NULL);
        free(*node);
    } else if (hasLeft(*node) && !hasRight(*node)) {
        // 2. Deleting a node with one child:
        // Remove the node and replace it with its child.
        updateParent(node, (*node)->left);
        free(*node);
    } else if (!hasLeft(*node) && hasRight(*node)) {
        updateParent(node, (*node)->right);
        free(*node);
    } else {  //  hasLeft(*node) && hasRight(*node)
        // 3. Deleting a node with two children:
        // Do not delete "node". Find the smallest (the leftest) child of
        // the right node's subtree (call him "m") and copy from "m" to "node"
        // all data (not including left, right or parent pointers) to "node".
        // Remove m.
        Node *m = findMin((*node)->right);
        (*node)->data = m->data;
        removeNode(&m);
    }
}

void removeNodeByData(int data, Node *root)
{
    Node *f = search(root, data);

    if (f == NULL)
        fprintf(stderr, "Unable to find node %d\n", data);
    else
        removeNode(&f);
}

void printNode(Node *node, int level)
{
    int i;
    //for (i = 0; i < level * TABWIDTH; ++i)
    for (i = level * TABWIDTH; i > 0; --i)
        printf(" ");

    if (node == NULL)
        printf("NULL\n");
    else
        printf("%d\n", node->data);
}

void printTree(Node *root)
{
    printf("\nPreorder traversal:\n");
    traversalPreorder(root, &printNode, 0);
}

void printTreeByLevel(Node *root)
{
    printf("\nQueue-based level order traversal:\n");
    traversalLevelorder(root, &printNode); 
}

void freeNode(Node *node, int level)
{
    if (node != NULL)
        free(node);
}

void freeTree(Node **root)
{
    if (root == NULL)
        return;

    traversalPostorder(*root, &freeNode, 0);
    root = NULL;
}
