#include "bintree.h"
#include <stdlib.h>
#include "queue.h"
#include <stdio.h>
//#include <time.h>

int main()
{
    BinTree *tree = createBinTree();
    if (tree == NULL)
        return 1;

    // deletion example from http://habrahabr.ru/blogs/algorithm/65617/
    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    int i;
    for (i = 0; i < 9; ++i)
        insert(a[i], tree);

    printTree(tree);
    removeNodeByData(5, tree);
    printTreeByLevel(tree);

    freeTree(&tree);
    return 0;
}
