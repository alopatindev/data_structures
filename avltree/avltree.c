#include "avltree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct Node* new_node(int data) {
    struct Node* root = (struct Node*) calloc(1, sizeof(struct Node));
    root->data = data;
    return root;
}

static void print_tree_internal(struct Node* root, int level) {
    for (int i = 0; i < level; i++) {
        printf("   ");
    }

    if (root == NULL) {
        printf("(null)\n");
    } else {
        printf("%d (height=%d parent=%p parent->data=%d)\n", root->data, root->height, (void *) root->parent, root->parent ? root->parent->data : -1);
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

    if (b != NULL) {
        b->parent = x;
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
            ASSERT(false, *root);
        }
    }

    if (b != NULL) {
        b->parent = y;
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

struct Node* find_node(struct Node* root, int data) {
    if (root == NULL) {
        return NULL;
    } else if (root->data == data) {
        return root;
    } else if (data < root->data) {
        return find_node(root->left, data);
    } else {
        return find_node(root->right, data);
    }
}

bool contains(struct Node* root, int data) {
    return find_node(root, data) != NULL;
}

struct Node* find_min(struct Node* root) {
    if (root == NULL) {
        return NULL;
    } else if (root->left != NULL) {
        return find_min(root->left);
    } else {
        return root;
    }
}

void remove_leaf(struct Node** root, struct Node* node) {
    const bool is_root = node == *root;
    struct Node* parent = node->parent;

    if (parent != NULL) {
        if (node == parent->left) {
            parent->left = NULL;
        } else if (node == parent->right) {
            parent->right = NULL;
        } else {
            ASSERT(false, *root);
        }

        update_height(parent);
    }

    if (is_root) {
        *root = NULL;
    }

    free(node);
}

void remove_node_with_left_child(struct Node** root, struct Node* node) {
    const bool is_root = node == *root;
    struct Node* child = node->left;

    if (node->parent != NULL) {
        if (node == node->parent->left) {
            node->parent->left = child;
        } else if (node == node->parent->right) {
            node->parent->right = child;
        } else {
            ASSERT(false, *root);
        }
    }

    node->left = NULL;
    child->left = node->left;
    child->right = node->right;
    child->parent = node->parent;

    if (is_root) {
        *root = child;
    }

    free(node);
}

void remove_node_with_right_child(struct Node** root, struct Node* node) {
    assert(false);
}

void remove_node_with_both_children(struct Node** root, struct Node* node) {
    assert(false);
}

void remove_node(struct Node** root, int data) {
    struct Node* node = find_node(*root, data);
    ASSERT(node != NULL, *root);

    if (node->left == NULL && node->right == NULL) {
        remove_leaf(root, node);
    } else if (node->left != NULL && node->right == NULL) {
        remove_node_with_left_child(root, node);
    } else if (node->left == NULL && node->right != NULL) {
        remove_node_with_right_child(root, node);
    } else {
        remove_node_with_both_children(root, node);
    }
}
