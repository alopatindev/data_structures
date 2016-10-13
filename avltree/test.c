#include "avltree.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// examples
// https://www.youtube.com/watch?v=FNeL18KsWPc&list=PLUl4u3cNGP61Oq3tWYp6V_F-5jb5L2iHb&index=6
// http://www.coe.utah.edu/~clillywh/CS2420/HW7/

static bool tree_is_balanced(int tree_size, int tree_height) {
    // https://en.wikipedia.org/wiki/AVL_tree#Comparison_to_other_structures
    if (tree_size == 0) {
        return true;
    }

    double height = (double) tree_height;
    double n = (double) tree_size;
    double b = -0.328;
    double c = 1.44;
    double d = 1.07;

    return height <= (c * log2(n + d) + b);
}

static void compute_tree_properties(struct Node* root, int* size, int* height, int level) {
    if (root != NULL) {
        (*size)++;
        level++;
        *height = MAX(level, *height);
        compute_tree_properties(root->left, size, height, level);
        compute_tree_properties(root->right, size, height, level);
    }
}

static void test_tree_properties(struct Node* root) {
    int tree_size = 0;
    int tree_height = 0;
    const int root_height = root != NULL ? root->height : -1;
    compute_tree_properties(root, &tree_size, &tree_height, 0);
    ASSERT(tree_height == root_height + 1, root);
    ASSERT(tree_is_balanced(tree_size, tree_height), root);
}

static void test_parent(struct Node* root, struct Node* node) {
    if (node == NULL) {
        return;
    }

    if (node->parent == NULL) {
        ASSERT(root == node, root);
    } else {
        const int data = node->data;
        bool found_self = false;

        if (node->parent->left != NULL) {
            found_self |= (data == node->parent->left->data);
        }

        if (node->parent->right != NULL) {
            found_self |= (data == node->parent->right->data);
        }

        ASSERT(found_self, root);
    }
}

void test_insert_rotate_left() {
    struct Node* root = NULL;

    insert(&root, 1);
    insert(&root, 2);

    ASSERT(1 == root->data, root);
    ASSERT(2 == root->right->data, root);

    test_parent(root, root);
    test_parent(root, root->right);

    test_tree_properties(root);

    insert(&root, 3);

    ASSERT(2 == root->data, root);
    ASSERT(1 == root->left->data, root);
    ASSERT(3 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);

    test_tree_properties(root);

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

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);

    test_tree_properties(root);

    insert(&root, 3);

    ASSERT(4 == root->data, root);
    ASSERT(3 == root->left->data, root);
    ASSERT(5 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);

    test_tree_properties(root);

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

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);

    test_tree_properties(root);

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

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_insert_sequential_ascending() {
    struct Node* root = NULL;

    const int size = 100;

    for (int i = 1; i <= size; i++) {
        insert(&root, i);
        test_parent(root, find_node(root, i));
    }

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_insert_sequential_descending() {
    struct Node* root = NULL;

    const int size = 100;

    for (int i = size; i >= 1; i--) {
        insert(&root, i);
        test_parent(root, find_node(root, i));
    }

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_insert_complex_1() {
    struct Node* root = NULL;
    test_tree_properties(root);

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

    test_tree_properties(root);

    insert(&root, 23);
    ASSERT(26 == root->left->right->data, root);
    ASSERT(23 == root->left->right->left->data, root);
    ASSERT(29 == root->left->right->right->data, root);

    test_tree_properties(root);

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

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);
    test_parent(root, root->left->left);
    test_parent(root, root->left->right);
    test_parent(root, root->right->left);
    test_parent(root, root->right->right);
    test_parent(root, root->left->right->left);
    test_parent(root, root->left->right->right);

    test_tree_properties(root);

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

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_insert_random() {
    struct Node* root = NULL;

    for (int i = 0; i <= 10000; i++) {
        int value = rand();

        if (!contains(root, value)) {
            insert(&root, value);

            struct Node* found = find_node(root, value);
            ASSERT(value == found->data, root);
            test_parent(root, found);

            test_tree_properties(root);
        }
    }

    free_tree(root);
    root = NULL;
}

void test_remove_leaf() {
    struct Node* root = NULL;

    insert(&root, 2);
    insert(&root, 1);
    insert(&root, 3);

    remove_node(&root, 3);
    ASSERT(!contains(root, 3), root);
    ASSERT(contains(root, 2), root);
    ASSERT(contains(root, 1), root);
    test_tree_properties(root);

    remove_node(&root, 1);
    ASSERT(!contains(root, 1), root);
    ASSERT(!contains(root, 3), root);
    ASSERT(contains(root, 2), root);
    test_tree_properties(root);

    remove_node(&root, 2);
    ASSERT(!contains(root, 2), root);
    ASSERT(!contains(root, 1), root);
    ASSERT(!contains(root, 3), root);
    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_node_with_left_child() {
    struct Node* root = NULL;

    insert(&root, 41);
    insert(&root, 20);
    insert(&root, 65);
    insert(&root, 11);
    insert(&root, 29);
    insert(&root, 50);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->left->data, root);

    remove_node(&root, 65);

    test_tree_properties(root);
    ASSERT(!contains(root, 65), root);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->data, root);

    free_tree(root);
    root = NULL;
}

void test_remove_node_with_right_child() {
    struct Node* root = NULL;

    insert(&root, 41);
    insert(&root, 20);
    insert(&root, 65);
    insert(&root, 11);
    insert(&root, 29);
    insert(&root, 70);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(70 == root->right->right->data, root);

    remove_node(&root, 65);

    test_tree_properties(root);
    ASSERT(!contains(root, 65), root);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(70 == root->right->data, root);

    ASSERT(2 == root->height, root);
    ASSERT(1 == root->left->height, root);
    ASSERT(0 == root->left->left->height, root);
    ASSERT(0 == root->left->right->height, root);
    ASSERT(0 == root->right->height, root);

    free_tree(root);
    root = NULL;
}

void test_remove_node_with_both_children_without_rebalance() {
    struct Node* root = NULL;

    insert(&root, 41);
    insert(&root, 20);
    insert(&root, 65);
    insert(&root, 11);
    insert(&root, 29);
    insert(&root, 50);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->left->data, root);

    remove_node(&root, 41);

    test_tree_properties(root);
    ASSERT(!contains(root, 41), root);

    ASSERT(50 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(65 == root->right->data, root);

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);
    test_parent(root, root->left->left);
    test_parent(root, root->left->right);

    free_tree(root);
    root = NULL;
}

void test_remove_node_with_both_children_with_rebalance() {
    struct Node* root = NULL;

    insert(&root, 50);
    insert(&root, 45);
    insert(&root, 92);
    insert(&root, 26);
    insert(&root, 46);
    insert(&root, 83);
    insert(&root, 99);
    insert(&root, 16);
    insert(&root, 38);

    remove_node(&root, 45);

    test_tree_properties(root);
    ASSERT(!contains(root, 45), root);

    ASSERT(50 == root->data, root);
    ASSERT(26 == root->left->data, root);
    ASSERT(92 == root->right->data, root);
    ASSERT(16 == root->left->left->data, root);
    ASSERT(46 == root->left->right->data, root);
    ASSERT(83 == root->right->left->data, root);
    ASSERT(99 == root->right->right->data, root);
    ASSERT(38 == root->left->right->left->data, root);

    test_parent(root, root);
    test_parent(root, root->left);
    test_parent(root, root->right);
    test_parent(root, root->left->left);
    test_parent(root, root->left->right);
    test_parent(root, root->right->left);
    test_parent(root, root->right->right);
    test_parent(root, root->left->right->left);

    free_tree(root);
    root = NULL;
}

void test_remove_random() {
    #define N 100

    bool added[N] = {false};
    struct Node* root = NULL;

    for (int i = 0; i <= N * N; i++) {
        int value = rand() % N;

        if (!added[value]) {
            insert(&root, value);
            added[value] = true;
            ASSERT(contains(root, value), root);
        }
    }

    test_tree_properties(root);

    for (int i = 0; i <= N * N; i++) {
        int value = rand() % N;

        if (added[value]) {
            remove_node(&root, value);
            added[value] = false;
            ASSERT(!contains(root, value), root);
            test_tree_properties(root);
        }
    }

    free_tree(root);
    root = NULL;

    #undef N
}

void test_find_min() {
    struct Node* root = NULL;

    insert(&root, 50);
    insert(&root, 45);
    insert(&root, 92);
    insert(&root, 26);
    insert(&root, 46);
    insert(&root, 83);
    insert(&root, 99);
    insert(&root, 16);
    insert(&root, 38);

    test_tree_properties(root);

    ASSERT(16 == find_min(root)->data, root);
    ASSERT(16 == find_min(root->left)->data, root);
    ASSERT(83 == find_min(root->right)->data, root);
    ASSERT(46 == find_min(root->left->right)->data, root);
    ASSERT(83 == find_min(root->right->left)->data, root);
    ASSERT(NULL == find_min(NULL), NULL);

    free_tree(root);
    root = NULL;
}

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

    test_find_min();

    test_remove_leaf();
    test_remove_node_with_left_child();
    test_remove_node_with_right_child();
    test_remove_node_with_both_children_without_rebalance();
    test_remove_node_with_both_children_with_rebalance();
    test_remove_random();

    return 0;
}
