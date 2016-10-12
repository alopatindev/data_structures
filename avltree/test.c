#include "avltree.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// examples
// https://www.youtube.com/watch?v=FNeL18KsWPc&list=PLUl4u3cNGP61Oq3tWYp6V_F-5jb5L2iHb&index=6
// http://www.coe.utah.edu/~clillywh/CS2420/HW7/

static int tree_size(struct Node* root, int size) {
    if (root != NULL) {
        size++;
        size = tree_size(root->left, size);
        size = tree_size(root->right, size);
    }

    return size;
}

static bool tree_is_balanced(struct Node* root) {
    // https://en.wikipedia.org/wiki/AVL_tree#Comparison_to_other_structures
    double height = (double) (root->height + 1);
    double n = (double) tree_size(root, 0);
    double b = -0.328;
    double c = 1.44;
    return height < (c * log2(n + 2.0) + b);
}

void test_insert_rotate_left() {
    struct Node* root = NULL;

    insert(&root, 1);
    insert(&root, 2);

    ASSERT(1 == root->data, root);
    ASSERT(2 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->right->height, root);

    ASSERT(NULL == root->parent, root);
    ASSERT(NULL == root->left, root);
    ASSERT(root == root->right->parent, root);

    ASSERT(tree_is_balanced(root), root);

    insert(&root, 3);

    ASSERT(2 == root->data, root);
    ASSERT(1 == root->left->data, root);
    ASSERT(3 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    ASSERT(NULL == root->parent, root);
    ASSERT(root == root->left->parent, root);
    ASSERT(root == root->right->parent, root);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_rotate_right() {
    struct Node* root = NULL;

    insert(&root, 5);
    insert(&root, 4);

    ASSERT(5 == root->data, root);
    ASSERT(4 == root->left->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);

    ASSERT(NULL == root->parent, root);
    ASSERT(root == root->left->parent, root);

    ASSERT(tree_is_balanced(root), root);

    insert(&root, 3);

    ASSERT(4 == root->data, root);
    ASSERT(3 == root->left->data, root);
    ASSERT(5 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    ASSERT(NULL == root->parent, root);
    ASSERT(root == root->left->parent, root);
    ASSERT(root == root->right->parent, root);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_rotate_left_right_simple() {
    struct Node* root = NULL;

    insert(&root, 3);
    insert(&root, 1);
    insert(&root, 2);

    ASSERT(2 == root->data, root);
    ASSERT(1 == root->left->data, root);
    ASSERT(3 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    ASSERT(2 == root->left->parent->data, root);
    ASSERT(2 == root->right->parent->data, root);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_rotate_right_left_simple() {
    struct Node* root = NULL;

    insert(&root, 1);
    insert(&root, 3);
    insert(&root, 2);

    ASSERT(2 == root->data, root);
    ASSERT(1 == root->left->data, root);
    ASSERT(3 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    ASSERT(2 == root->left->parent->data, root);
    ASSERT(2 == root->right->parent->data, root);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_sequential_ascending() {
    struct Node* root = NULL;

    for (int i = 0; i <= 100; i++) {
        insert(&root, i);
        ASSERT(tree_is_balanced(root), root);
    }

    free_tree(root);
    root = NULL;
}

void test_insert_sequential_descending() {
    struct Node* root = NULL;

    for (int i = 100; i >= 0; i--) {
        insert(&root, i);
        ASSERT(tree_is_balanced(root), root);
    }

    free_tree(root);
    root = NULL;
}

void test_insert_complex_1() {
    struct Node* root = NULL;

    insert(&root, 41);
    insert(&root, 20);
    insert(&root, 65);
    insert(&root, 11);
    insert(&root, 29);
    insert(&root, 50);
    insert(&root, 26);

    ASSERT(contains(root, 20), root);
    ASSERT(contains(root, 65), root);
    ASSERT(contains(root, 26), root);
    ASSERT(!contains(root, 123), root);
    ASSERT(!contains(root, 0), root);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->left->data, root);
    ASSERT(26 == root->left->right->left->data, root);

    ASSERT(tree_is_balanced(root), root);

    insert(&root, 23);
    ASSERT(26 == root->left->right->data, root);
    ASSERT(23 == root->left->right->left->data, root);
    ASSERT(29 == root->left->right->right->data, root);

    ASSERT(tree_is_balanced(root), root);

    insert(&root, 55);
    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(55 == root->right->data, root);
    ASSERT(50 == root->right->left->data, root);
    ASSERT(65 == root->right->right->data, root);

    ASSERT(3 == root->height, root);
    ASSERT(2 == root->left->height, root);
    ASSERT(1 == root->right->height, root);
    ASSERT(0 == root->right->left->height, root);
    ASSERT(0 == root->right->right->height, root);

    ASSERT(41 == root->left->parent->data, root);
    ASSERT(41 == root->right->parent->data, root);
    ASSERT(20 == root->left->left->parent->data, root);
    ASSERT(20 == root->left->right->parent->data, root);
    ASSERT(55 == root->right->left->parent->data, root);
    ASSERT(55 == root->right->right->parent->data, root);
    ASSERT(26 == root->left->right->left->parent->data, root);
    ASSERT(26 == root->left->right->right->parent->data, root);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_complex_2() {
    struct Node* root = NULL;

    insert(&root, 50);
    insert(&root, 46);
    insert(&root, 92);
    insert(&root, 83);
    insert(&root, 16);
    insert(&root, 99);
    insert(&root, 43);
    insert(&root, 38);
    insert(&root, 26);
    insert(&root, 15);
    insert(&root, 39);
    insert(&root, 65);
    insert(&root, 45);
    insert(&root, 51);
    insert(&root, 79);
    insert(&root, 37);
    insert(&root, 35);
    insert(&root, 28);
    insert(&root, 60);
    insert(&root, 78);
    insert(&root, 76);

    ASSERT(tree_is_balanced(root), root);

    free_tree(root);
    root = NULL;
}

void test_insert_random() {
    struct Node* root = NULL;

    for (int i = 0; i <= 10000; i++) {
        int value = rand();

        if (!contains(root, value)) {
            insert(&root, value);
            ASSERT(root->left == NULL || root->left->parent == root, root);
            ASSERT(root->right == NULL || root->right->parent == root, root);
            ASSERT(contains(root, value), root);
        }

        ASSERT(tree_is_balanced(root), root);
    }

    free_tree(root);
    root = NULL;
}

// TODO: remove

int main()
{
    srand(time(NULL));
    test_insert_rotate_left();
    test_insert_rotate_right();
    test_insert_rotate_left_right_simple();
    test_insert_rotate_right_left_simple();
    test_insert_sequential_ascending();
    test_insert_sequential_descending();
    test_insert_complex_1();
    test_insert_complex_2();
    test_insert_random();
    return 0;
}
