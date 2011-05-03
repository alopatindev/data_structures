#include <stdio.h>
#include "rbtree.h"

int main()
{
    struct RBTree *tree = createRBTree();
    int i;
    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    for (i = 0; i < 9; ++i) {
        char tmp[3] = "";
        snprintf(tmp, 3, "%d", a[i]);
        insertRBNode(tree, tmp, &i);
        //insertRBNodeRecursive(tree, tmp, &i);
        //printf("pushed %d\n", a[i]);
    }
    traversalPreorder(tree->root, &printRBNode);
    //traversalPostorder(tree->root, &printRBNode);
//    traversalPostorderRecursive(tree->root, &printRBNode, 0);
    freeRBTree(&tree);

    return 0;
}
