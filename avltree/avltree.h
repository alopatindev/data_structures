#ifndef AVLTREE_H
#define AVLTREE_H

#include <stdbool.h>
#include <assert.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ASSERT(expr, root) do { \
    if (!(expr)) { \
        print_tree((root)); \
        assert(expr); \
    } \
} while (false)

struct Node {
    int data;
    int height;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
};

void print_tree(struct Node* root);
void free_tree(struct Node* root);

void insert(struct Node** root, int data);
bool contains(struct Node* root, int data);
void remove_node(struct Node** root, int data);

#endif
