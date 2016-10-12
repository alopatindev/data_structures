#include "avltree.h"
#include <assert.h>
#include <string.h>

void test_rotate_left() {
    struct Node* root = NULL;
    insert(&root, 1);
    insert(&root, 2);

    assert(1 == root->data);
    assert(2 == root->right->data);

    assert(1 == root->height);
    assert(0 == root->right->height);

    assert(NULL == root->parent);
    assert(NULL == root->left);
    assert(root == root->right->parent);

    insert(&root, 3);

    assert(2 == root->data);
    assert(1 == root->left->data);
    assert(3 == root->right->data);

    assert(1 == root->height);
    assert(0 == root->left->height);
    assert(0 == root->right->height);

    assert(NULL == root->parent);
    assert(root == root->left->parent);
    assert(root == root->right->parent);

    free_tree(root);
    root = NULL;
}

void test_rotate_right()
{
    struct Node* root = NULL;
    insert(&root, 5);
    insert(&root, 4);

    assert(5 == root->data);
    assert(4 == root->left->data);

    assert(1 == root->height);
    assert(0 == root->left->height);

    assert(NULL == root->parent);
    assert(root == root->left->parent);

    insert(&root, 3);

    assert(4 == root->data);
    assert(3 == root->left->data);
    assert(5 == root->right->data);

    assert(1 == root->height);
    assert(0 == root->left->height);
    assert(0 == root->right->height);

    assert(NULL == root->parent);
    assert(root == root->left->parent);
    assert(root == root->right->parent);

    free_tree(root);
    root = NULL;
}

void test_rotate_left_right()
{
    struct Node* root = NULL;
    insert(&root, 41);
    insert(&root, 20);
    insert(&root, 65);
    insert(&root, 11);
    insert(&root, 29);
    insert(&root, 50);
    insert(&root, 26);
    print_tree(root);

    assert(contains(root, 20));
    assert(contains(root, 65));
    assert(contains(root, 26));
    assert(!contains(root, 123));
    assert(!contains(root, 0));

    assert(41 == root->data);
    assert(20 == root->left->data);
    assert(65 == root->right->data);
    assert(11 == root->left->left->data);
    assert(29 == root->left->right->data);
    assert(50 == root->right->left->data);
    assert(26 == root->left->right->left->data);

    insert(&root, 23);
    assert(26 == root->left->right->data);
    assert(23 == root->left->right->left->data);
    assert(29 == root->left->right->right->data);

    insert(&root, 55);
    assert(41 == root->data);
    assert(20 == root->left->data);
    assert(55 == root->right->data);
    assert(50 == root->right->left->data);
    assert(65 == root->right->right->data);

    assert(3 == root->height);
    assert(2 == root->left->height);
    assert(1 == root->right->height);
    assert(0 == root->right->left->height);
    assert(0 == root->right->right->height);

    free_tree(root);
    root = NULL;

    // TODO: check heights
    // TODO: check parents
}

void test_rotate_right_left()
{
    // TODO
}

// TODO: remove
// TODO: randomized input; check that tree is balanced

int main()
{
    test_rotate_left();
    test_rotate_right();
    test_rotate_left_right();
    test_rotate_right_left();
    return 0;
}
