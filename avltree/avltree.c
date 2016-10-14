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
        printf("%d (%p height=%d parent=%p parent->data=%d)\n",
                root->data,
                (void*) root,
                root->height,
                (void*) root->parent,
                root->parent != NULL ? root->parent->data : -1);
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

static void swap_parent(struct Node** root, struct Node* node, struct Node* other) {
    const bool is_root = node == *root;

    if (node != NULL && node->parent != NULL) {
        if (node == node->parent->left) {
            node->parent->left = other;
        } else if (node == node->parent->right) {
            node->parent->right = other;
        } else {
            assert(false);
        }
    }

    if (other != NULL) {
        other->parent = node->parent;
    }

    if (is_root) {
        *root = other;
    }
}

static void rotate_left(struct Node** root, struct Node* x) {
    struct Node* y = x->right;
    struct Node* b = y->left;

    swap_parent(root, x, y);

    if (b != NULL) {
        b->parent = x;
    }

    y->left = x;
    x->parent = y;
    x->right = b;

    update_height(x);
    update_height(y);
}

static void rotate_right(struct Node** root, struct Node* y) {
    struct Node* x = y->left;
    struct Node* b = x->right;

    swap_parent(root, y, x);

    if (b != NULL) {
        b->parent = y;
    }

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

static void remove_leaf(struct Node** root, struct Node* node) {
    swap_parent(root, node, NULL);

    update_height(node);
    update_balance(root, node);

    free(node);
}

static void remove_node_with_left_child(struct Node** root, struct Node* node) {
    struct Node* child = node->left;

    swap_parent(root, node, child);

    child->right = node->right;

    struct Node* lowest = child->right != NULL ? child->right : child;
    update_height(lowest);
    update_balance(root, lowest);

    free(node);
}

static void remove_node_with_right_child(struct Node** root, struct Node* node) {
    struct Node* child = node->right;

    swap_parent(root, node, child);

    child->left = node->left;

    struct Node* lowest = child->left != NULL ? child->left : child;
    update_height(lowest);
    update_balance(root, lowest);

    free(node);
}

static struct Node* swap_and_separate_nodes(struct Node** root, struct Node* node, struct Node* child) {
    ASSERT(child != NULL, *root);

    assert(NULL == child->left);

    // separate child
    child->right = NULL;
    if (child->parent->left == child) {
        child->parent->left = NULL;
    } else if (child->parent->right == child) {
        child->parent->right = NULL;
    } else {
        assert(false);
    }
    child->parent = NULL;

    struct Node* replaced_node = child;
    child = NULL;

    // separate node (1)
    node->left = NULL;
    node->right = NULL;

    // replace node with child
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            node->parent->left = replaced_node;
        } else if (node->parent->right == node) {
            node->parent->right = replaced_node;
        } else {
            assert(false);
        }
    }

    if (node == *root) {
        *root = replaced_node;
    }

    // separate node (2)
    node->parent = NULL;

    return replaced_node;
}

static void remove_node_with_both_children(struct Node** root, struct Node* node) {
    struct Node* child = find_min(node->right);

    struct Node* node_left = node->left;
    struct Node* node_right = node->right;
    struct Node* node_parent = node->parent;
    struct Node* child_left = child->left;
    struct Node* child_right = child->right;
    struct Node* child_parent = child->parent;

    ASSERT(NULL == child_left, *root);

    struct Node* replaced_node = swap_and_separate_nodes(root, node, child);

    replaced_node->left =
        node_left == replaced_node
        ? NULL : node_left;

    replaced_node->right =
        node_right == replaced_node
        ? child_right : node_right;

    replaced_node->parent = node_parent;

    if (replaced_node->left != NULL) {
        replaced_node->left->parent = replaced_node;
    }

    if (replaced_node->right != NULL) {
        replaced_node->right->parent = replaced_node;
    }

    if (child_right != NULL && child_parent != node) {
        child_parent->left = child_right;
        child_right->parent = child_parent;
    }

	if (child_right != NULL) {
        update_height(child_right);
        update_balance(root, child_right);
    } else {
        update_height(child_parent);
        update_balance(root, child_parent);
    }

    update_height(replaced_node->left);
    update_height(replaced_node->right);
    update_balance(root, replaced_node);

    ASSERT(node != NULL, *root);
    free(node);
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
