#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "stack.h"
#include "queue.h"

STACK(StackHLevels, int)
STACK(StackHNodes, struct HNode *)
QUEUE(QueueHLevels, int)
QUEUE(QueueHNodes, struct HNode *)

int compareKeys(char *key1, char *key2) // operator<
{
    size_t l1 = strnlen(key1, MAXKEYSIZE), l2 = strnlen(key2, MAXKEYSIZE);

    if (l1 > l2)
        return 0;
    else if (l1 < l2)
        return 1;
    else
        return strncmp(key1, key2, MAXKEYSIZE) < 0;
}

int equalKeys(char *key1, char *key2)
{
    size_t i;
    for (i = 0; i < MAXKEYSIZE; ++i)
        if (key1[i] != key2[i])
            return 0;
    return 1;
}

struct Heap *createHeap()
{
    struct Heap *heap = (struct Heap *)malloc(sizeof(struct Heap));
    if (!heap) {
        fprintf(stderr, "Failed to allocate memory for a heap\n");
        return NULL;
    }

    heap->root = NULL;
    heap->size = 0;
    heap->height = -1;

    return heap;
}

struct HNode *createHNode(char *key, HDATATYPE *data, struct HNode *parent,
                          struct HNode *left, struct HNode *right)
{
    struct HNode *node = (struct HNode *)malloc(sizeof(struct HNode));
    if (!node) {
        fprintf(stderr, "Failed to allocate memory for a heap node\n");
        return NULL;
    }

    //*(node->key) = &(key[0]);
    strncpy(node->key, key, MAXKEYSIZE);
    //node->data = *data;
    memcpy(&(node->data), data, sizeof(HDATATYPE));
    node->parent = parent;
    node->left = left;
    node->right = right;

    return node;
}

void swap(struct Heap *heap, struct HNode *h1, struct HNode *h2)
{
    // FIXME: swap pointers, not data
    if (!h1 || !h2 || !heap)
        return;

    //printf("swapping '%s' with '%s'\n", h1->key, h2->key);

    char k[MAXKEYSIZE];
    strncpy(k, h2->key, MAXKEYSIZE);
    strncpy(h2->key, h1->key, MAXKEYSIZE);
    strncpy(h1->key, k, MAXKEYSIZE);

    HDATATYPE data;
    data = h2->data;
    h2->data = h1->data;
    h1->data = data;
}

void heapFixup(struct Heap *heap, struct HNode *n)
{
    while (n->parent && compareKeys(n->parent->key, n->key)) {
        swap(heap, n->parent, n);
        n = n->parent;
    }
}

void insertHNode(struct Heap *heap, char *key, HDATATYPE *data)
{
    struct HNode *n = insertHNodeBinary(heap, key, data);
    if (!n)
        return;
    heapFixup(heap, n);
}

struct HNode *insertHNodeBinary(struct Heap *heap, char *key, HDATATYPE *data)
{
    if (!heap->root) {
        heap->root = createHNode(key, data, NULL, NULL, NULL);
        return heap->root;
    }

    struct HNode *node = heap->root, *parent = NULL;

    int height;
    for (height = 0; node; ++height) {
        if (equalKeys(key, node->key)) {
            return NULL;
        } else {
            parent = node;
            node = compareKeys(key, node->key) ? node->left : node->right;
        }
    }

    if (height > heap->height)
        heap->height++;

    struct HNode *n = createHNode(key, data, parent, NULL, NULL);
    if (!n)
        return NULL;
    
    if (compareKeys(key, parent->key))
        parent->left = n;
    else
        parent->right = n;

    heap->size++;

    return n;
}

void traversalPreorder(struct HNode *root,
                       int (*f)(struct HNode *, int, void *), void *adata)
{
    struct StackHNodes *nodes = createStackHNodes();
    struct StackHLevels *levels = createStackHLevels();
    struct HNode *node = root;
    int height = 0;
    pushStackHNodes(nodes, &node);
    pushStackHLevels(levels, &height);
    while (!isEmptyStackHNodes(nodes)) {
        struct LISTStackHNodes *n = popStackHNodes(nodes);
        struct LISTStackHLevels *h = popStackHLevels(levels);
        node = n->data;
        height = h->data;
        free(n); free(h);

        if (!f(node, height, adata))
            break;

        if (node->right) {
            int h = height + 1;
            pushStackHNodes(nodes, &(node->right));
            pushStackHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
        if (node->left) {
            int h = height + 1;
            pushStackHNodes(nodes, &(node->left));
            pushStackHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
    }
    freeStackHNodes(&nodes);
    freeStackHLevels(&levels);
}

void traversalInorder(struct HNode *root,
                      int (*f)(struct HNode *, int, void *), void *adata)
{
    struct StackHNodes *nodes = createStackHNodes();
    struct StackHLevels *levels = createStackHLevels();

    struct HNode *node = root;
    int height = -1;

    while (!isEmptyStackHNodes(nodes) || node) {
        if (node) {
            pushStackHNodes(nodes, &node);
            int h = height + 1;
            pushStackHLevels(levels, &h);
            node = node->left;
            ++height;
        } else {
            if (!f(NULL, height + 1, adata))
                break;
            struct LISTStackHNodes *n = popStackHNodes(nodes);
            struct LISTStackHLevels *l = popStackHLevels(levels);
            node = n->data;
            height = l->data;
            free(n); free(l);
            if (!f(node, height, adata))
                break;
            node = node->right;
        }
    }
    f(NULL, height + 1, adata);  // FIXME
    freeStackHNodes(&nodes);
    freeStackHLevels(&levels);
}

void traversalPostorder(struct HNode *root,
                        int (*f)(struct HNode *, int, void *), void *adata)
{
    struct StackHNodes *nodes = createStackHNodes();
    struct StackHLevels *levels = createStackHLevels();
    struct HNode *node = root;
    int height = 0;

    while (node || !isEmptyStackHNodes(nodes)) {
        if (!node) {
            if (!f(NULL, height, adata))
                break;
            if (!(topStackHNodes(nodes)->data->right)) {
                if (!f(NULL, height, adata))
                    break;
            }

            while (!isEmptyStackHNodes(nodes) &&
                   topStackHNodes(nodes)->data->right == node)
            {
                struct LISTStackHNodes *n = popStackHNodes(nodes);
                struct LISTStackHLevels *l = popStackHLevels(levels);
                node = n->data;
                height = l->data;
                free(n); free(l);
                if (!f(node, height, adata))
                    break;
            }
            node = isEmptyStackHNodes(nodes)
                   ? NULL : topStackHNodes(nodes)->data->right;
        }
        if (node) {
            ++height;
            pushStackHNodes(nodes, &node);
            int h = height-1;
            pushStackHLevels(levels, &h);
            node = node->left;
        }
    }
    freeStackHNodes(&nodes);
    freeStackHLevels(&levels);
}

void traversalLevelorder(struct HNode *root,
                         int (*f)(struct HNode *, int, void *), void *adata)
{
    struct QueueHNodes *nodes = createQueueHNodes();
    struct QueueHLevels *levels = createQueueHLevels();
    int h = 0;
    pushQueueHNodes(nodes, &root);
    pushQueueHLevels(levels, &h);
    while (!isEmptyQueueHNodes(nodes)) {
        struct LISTQueueHNodes *n = popQueueHNodes(nodes);
        struct LISTQueueHLevels *l = popQueueHLevels(levels);
        struct HNode *node = n->data;
        int height = l->data;
        free(n); free(l);
        if (!f(node, height, adata))
            break;
        if (node->left) {
            h = height + 1;
            pushQueueHNodes(nodes, &(node->left));
            pushQueueHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
        if (node->right) {
            h = height + 1;
            pushQueueHNodes(nodes, &(node->right));
            pushQueueHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
    }
    freeQueueHNodes(&nodes);
    freeQueueHLevels(&levels);
}

void traversalLevelorder2(struct HNode *root,
                          int (*f)(struct HNode *, int, void *), void *adata)
{
    struct QueueHNodes *nodes = createQueueHNodes();
    struct QueueHLevels *levels = createQueueHLevels();
    int h = 0;
    pushQueueHNodes(nodes, &root);
    pushQueueHLevels(levels, &h);
    while (!isEmptyQueueHNodes(nodes)) {
        struct LISTQueueHNodes *n = popQueueHNodes(nodes);
        struct LISTQueueHLevels *l = popQueueHLevels(levels);
        struct HNode *node = n->data;
        int height = l->data;
        free(n); free(l);
        if (!f(node, height, adata))
            break;
        if (node->right) {
            h = height + 1;
            pushQueueHNodes(nodes, &(node->right));
            pushQueueHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
        if (node->left) {
            h = height + 1;
            pushQueueHNodes(nodes, &(node->left));
            pushQueueHLevels(levels, &h);
        } else {
            if (!f(NULL, height + 1, adata))
                break;
        }
    }
    freeQueueHNodes(&nodes);
    freeQueueHLevels(&levels);
}

struct HNode *searchHeap(struct Heap *heap, char *key)
{
    struct PKeyNode pkn;
    strncpy(pkn.key, key, MAXKEYSIZE);
    traversalLevelorder(heap->root, &notMatchHNode, (void *)(&pkn));
    return pkn.node;
}

int notMatchHNode(struct HNode *node, int height, void *adata)
{
    struct PKeyNode *pkn = (struct PKeyNode *)adata;
    return !(node && equalKeys(node->key, pkn->key) && (pkn->node = node));
}

int freeHNode(struct HNode *node, int height, void *adata)
{
    if (node)
        free((void *)node);
    return 1;
}

void clearHeap(struct Heap *heap)
{
    if (!heap || !heap->root || !heap->size)
        return;

    traversalPostorder(heap->root, &freeHNode, NULL);
    heap->size = 0;
}

void freeHeap(struct Heap **heap)
{
    clearHeap(*heap);
    free((void *)*heap);
    heap = NULL;
}

int printHNode(struct HNode *node, int height, void *adata)
{
    int i;
    for (i = height * TABWIDTH; i > 0; --i)
        printf(" ");

    if (node == NULL)
        printf("NULL\n");
    else
        printf(PRINTFORMAT, node->key, node->data);

    return 1;
}

void printHeap(struct Heap *heap)
{
    traversalPreorder(heap->root, &printHNode, NULL);
}
