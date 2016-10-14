#include "avltree.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// examples
// https://www.youtube.com/watch?v=FNeL18KsWPc&list=PLUl4u3cNGP61Oq3tWYp6V_F-5jb5L2iHb&index=6
// http://www.coe.utah.edu/~clillywh/CS2420/HW7/

#define BUILD_TREE(items) \
    for (size_t i = 0; i < sizeof(items) / sizeof(items[0]); i++) { \
        insert(&root, items[i]); \
    }

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

static void test_parent(struct Node* root, struct Node* node) {
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

static void test_all_parents(struct Node* root, struct Node* node) {
    if (node != NULL) {
        test_parent(root, node);
        test_all_parents(root, node->left);
        test_all_parents(root, node->right);
    }
}

static void test_tree_properties(struct Node* root) {
    int tree_size = 0;
    int tree_height = 0;
    const int root_height = root != NULL ? root->height : -1;
    compute_tree_properties(root, &tree_size, &tree_height, 0);
    ASSERT(tree_height == root_height + 1, root);
    ASSERT(tree_is_balanced(tree_size, tree_height), root);
    test_all_parents(root, root);
}

void test_insert_rotate_left() {
    struct Node* root = NULL;

    insert(&root, 1);
    insert(&root, 2);

    ASSERT(1 == root->data, root);
    ASSERT(2 == root->right->data, root);

    test_tree_properties(root);

    insert(&root, 3);

    ASSERT(2 == root->data, root);
    ASSERT(1 == root->left->data, root);
    ASSERT(3 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

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

    test_tree_properties(root);

    insert(&root, 3);

    ASSERT(4 == root->data, root);
    ASSERT(3 == root->left->data, root);
    ASSERT(5 == root->right->data, root);

    ASSERT(1 == root->height, root);
    ASSERT(0 == root->left->height, root);
    ASSERT(0 == root->right->height, root);

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

    int items[] = {41, 20, 65, 11, 29, 50, 26};
    BUILD_TREE(items);

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

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_insert_complex_2() {
    struct Node* root = NULL;

    int items[] = {50, 46, 92, 83, 16, 99, 43, 38, 26, 15, 39, 65, 45, 51, 79, 37, 35, 28, 60, 78, 76};
    BUILD_TREE(items);

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

    int items[] = {41, 20, 65, 11, 29, 50};
    BUILD_TREE(items);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->left->data, root);

    remove_node(&root, 65);
    ASSERT(!contains(root, 65), root);

    test_tree_properties(root);

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

    int items[] = {41, 20, 65, 11, 29, 70};
    BUILD_TREE(items);

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

    int items[] = {41, 20, 65, 11, 29, 50};
    BUILD_TREE(items);

    ASSERT(41 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(65 == root->right->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(50 == root->right->left->data, root);

    remove_node(&root, 41);
    ASSERT(!contains(root, 41), root);

    test_tree_properties(root);

    ASSERT(50 == root->data, root);
    ASSERT(20 == root->left->data, root);
    ASSERT(11 == root->left->left->data, root);
    ASSERT(29 == root->left->right->data, root);
    ASSERT(65 == root->right->data, root);

    free_tree(root);
    root = NULL;
}

void test_remove_leaf_with_rebalance() {
    struct Node* root = NULL;

    int items[] = {6, 5, 7, 4};
    BUILD_TREE(items);

    remove_node(&root, 7);
    ASSERT(!contains(root, 7), root);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_random() {
    #define N 100

    bool added[N] = {false};
    struct Node* root = NULL;

    bool add = false;
    for (int i = 0; i <= N * N * N; i++) {
        int value = rand() % N;

        if (i % 5 == 0) {
            add = (rand() % 2) == 0;
        }

        if (add && !added[value]) {
            insert(&root, value);
            added[value] = true;
            ASSERT(contains(root, value), root);
        }

        if (!add && added[value]) {
            remove_node(&root, value);
            added[value] = false;
            ASSERT(!contains(root, value), root);
        }

        test_tree_properties(root);
    }

    free_tree(root);
    root = NULL;

    #undef N
}

void test_find_min() {
    struct Node* root = NULL;

    int items[] = {50, 45, 92, 26, 46, 83, 99, 16, 38};
    BUILD_TREE(items);

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

void test_remove_complex_1() {
    struct Node* root = NULL;

    int items[] = {86, 27, 75, 41, 64, 88, 11, 50, 28, 30};
    BUILD_TREE(items);

    test_tree_properties(root);

    remove_node(&root, 30);
    remove_node(&root, 75);
    remove_node(&root, 88);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_2() {
    struct Node* root = NULL;

    int items[] = {60, 44, 69, 67, 91, 89, 98};
    BUILD_TREE(items);

    remove_node(&root, 69);
    remove_node(&root, 91);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_3() {
    struct Node* root = NULL;

    int items[] = {75, 85, 30, 14, 15, 23, 12};
    BUILD_TREE(items);

    remove_node(&root, 85);
    remove_node(&root, 15);
    remove_node(&root, 75);
    remove_node(&root, 23);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_4() {
    struct Node* root = NULL;

    int items[] = {44, 96, 76, 45, 20, 16, 28};
    BUILD_TREE(items);

    remove_node(&root, 20);
    remove_node(&root, 45);
    remove_node(&root, 28);
    remove_node(&root, 76);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_5() {
    struct Node* root = NULL;

    int items[] = {72, 76, 35, 56, 91, 20, 22, 27};
    BUILD_TREE(items);

    remove_node(&root, 27);
    remove_node(&root, 20);
    remove_node(&root, 91);
    remove_node(&root, 72);
    remove_node(&root, 35);
    remove_node(&root, 76);

    remove_node(&root, 56);
    remove_node(&root, 22);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_root() {
    struct Node* root = NULL;

    int items[] = {72, 42, 58, 71, 39, 62, 30, 17, 67};
    BUILD_TREE(items);

    remove_node(&root, 58);
    ASSERT(71 == root->right->data, root);
    ASSERT(72 == root->right->right->data, root);
    ASSERT(67 == root->right->left->data, root);
    remove_node(&root, 71);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_non_root() {
    struct Node* root = NULL;

    int items[] = {62, 39, 71, 30, 42, 67, 72};
    BUILD_TREE(items);

    remove_node(&root, 71);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_6() {
    struct Node* root = NULL;

    int items[] = {63, 85, 26, 15, 19, 4, 73, 60, 83};
    BUILD_TREE(items);

    remove_node(&root, 26);
    remove_node(&root, 19);
    test_tree_properties(root);
    remove_node(&root, 63);
    remove_node(&root, 4);
    remove_node(&root, 60);
    remove_node(&root, 15);
    remove_node(&root, 85);

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_remove_complex_7() {
    struct Node* root = NULL;
    int items[] = {11, 22, 45, 69, 52, 46, 71, 51, 75};

    BUILD_TREE(items);
    test_tree_properties(root);

    int removable[] = {45, 69, 75, 11, 71, 51};

    for (size_t i = 0; i < sizeof(removable) / sizeof(removable[0]); i++) {
        remove_node(&root, removable[i]);
    }

    test_tree_properties(root);

    free_tree(root);
    root = NULL;
}

void test_balance_after_removal() {
    struct Node* root = NULL;
    int items[] = {55, 33, 82, 47, 27, 86, 69, 25, 59};
    BUILD_TREE(items);
    test_tree_properties(root);

    int removable[] = {55, 27};

    for (size_t i = 0; i < sizeof(removable) / sizeof(removable[0]); i++) {
        remove_node(&root, removable[i]);
    }

    test_tree_properties(root);

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
    test_remove_leaf_with_rebalance();
    test_remove_complex_1();
    test_remove_complex_2();
    test_remove_complex_3();
    test_remove_complex_4();
    test_remove_complex_5();
    test_remove_complex_6();
    test_remove_complex_7();
    test_remove_complex_root();
    test_remove_complex_non_root();
    test_balance_after_removal();
    test_remove_random();

    return 0;
}
