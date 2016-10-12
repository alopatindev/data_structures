#include "avltree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

static struct Node* new_node(int data) {
    struct Node* root = (struct Node*) calloc(1, sizeof(struct Node));
    root->data = data;
    return root;
}

static void print_tree_internal(struct Node* root, int level) {
    for (int i = 0; i < level; i++) {
        printf(" ");
    }

    if (root == NULL) {
        printf("(null)\n");
    } else {
        printf("%d (height=%d)\n", root->data, root->height);
        print_tree_internal(root->left, level + 1);
        print_tree_internal(root->right, level + 1);
    }
}

void print_tree(struct Node* root) {
    print_tree_internal(root, 0);
    printf("\n");
}

void free_tree(struct Node* root) {
    if (root != NULL) {
        if (root->left != NULL) {
            free_tree(root->left);
        }

        if (root->right != NULL) {
            free_tree(root->right);
        }

        free(root);
    }
}

static void update_height(struct Node* node) {
    if (node != NULL) {
        if (node->left != NULL && node->right != NULL) {
            node->height = MAX(node->left->height, node->right->height) + 1;
        } else if (node->left != NULL) {
            node->height = node->left->height + 1;
        } else if (node->right != NULL) {
            node->height = node->right->height + 1;
        } else {
            node->height = 0;
        }
        update_height(node->parent);
    }
}

static void rotate_left(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    struct Node* b = y->left;

    if (x == *root) {
        *root = y;
    }

    if (x->parent != NULL) {
        if (x->parent->left == x) {
            x->parent->left = y;
        } else if (x->parent->right == x) {
            x->parent->right = y;
        } else {
            ASSERT(false, *root);
        }
    }

    y->parent = x->parent;
    y->left = x;
    x->parent = y;
    x->right = b;

    update_height(x);
    update_height(y);
}

static void rotate_right(struct Node** root, struct Node* y) {
    struct Node* x = y->left;
    struct Node* b = x->right;

    if (y == *root) {
        *root = x;
    }

    if (y->parent != NULL) {
        if (y->parent->left == y) {
            y->parent->left = x;
        } else if (y->parent->right == y) {
            y->parent->right = x;
        } else {
            printf("y=%p (data=%d parent=%p)\n", (void *)y, y->data, (void *)y->parent);
            ASSERT(false, *root);
        }
    }

    x->parent = y->parent;
    y->parent = x;
    y->left = b;
    x->right = y;

    update_height(y);
    update_height(x);
}

static int get_height(struct Node* node) {
    return node != NULL ? node->height : -1;
}

static int get_height_diff(struct Node* node) {
    if (node == NULL) {
        return 0;
    } else {
        return get_height(node->left) - get_height(node->right);
    }
}

static void update_balance(struct Node** root, struct Node* node) {
    if (node != NULL) {
        int height_diff = get_height_diff(node);
        if (height_diff > 1) {
            bool needs_double_rotation = node->height > 1 && get_height_diff(node->left) < 0;
            if (needs_double_rotation) {
                rotate_left(root, node->left);
                rotate_right(root, node);
            } else {
                rotate_right(root, node);
            }
        } else if (height_diff < -1) {
            bool needs_double_rotation = node->height > 1 && get_height_diff(node->right) > 0;
            if (needs_double_rotation) {
                rotate_right(root, node->right);
                rotate_left(root, node);
            } else {
                rotate_left(root, node);
            }
        }

        update_balance(root, node->parent);
    }
}

static void insert_internal(struct Node** root, struct Node* node, int data) {
    if (*root == NULL) {
        *root = new_node(data);
        return;
    }

    ASSERT(data != node->data, *root);

    if (data < node->data) {
        if (node->left == NULL) {
            node->left = new_node(data);
            node->left->parent = node;
            update_height(node);
            update_balance(root, node->parent);
        } else {
            insert_internal(root, node->left, data);
        }
    } else {
        if (node->right == NULL) {
            node->right = new_node(data);
            node->right->parent = node;
            update_height(node);
            update_balance(root, node->parent);
        } else {
            insert_internal(root, node->right, data);
        }
    }
}

void insert(struct Node** root, int data) {
    insert_internal(root, *root, data);
}

bool contains(struct Node* root, int data) {
    if (root == NULL) {
        return false;
    } else {
        if (data == root->data) {
            return true;
        } else {
            return contains(root->left, data) || contains(root->right, data);
        }
    }
}

void remove_node(struct Node** root, int data) {
}
