#include <stdio.h>
#include "rbtree.h"

int main()
{
    printf("33<5=%d\n",compare("33", "5"));
    struct RBTree *tree = createRBTree();
    int i;
    //int a[] = {3, 5, 1, 4, 2, 7, 0, 8, 9};
    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    for (i = 0; i < 9; ++i) {
        char tmp[3] = "";
        snprintf(tmp, 3, "%d", a[i]);
        insertRBNode(tree, tmp, &i);
        //insertRBNodeRecursive(tmp, &i, tree);
        //printf("pushed %d\n", a[i]);
    }
    traversalPreorder(tree->root, &printRBNode);
    //traversalPostorder(tree->root, &printRBNode);
//    traversalPostorderRecursive(tree->root, &printRBNode, 0);
    printf("tree's height=%d\n", tree->height);
    freeRBTree(&tree);

    return 0;
}
