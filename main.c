#include "bintree.h"
#include <stdlib.h>

int main()
{
    // deletion example from http://habrahabr.ru/blogs/algorithm/65617/
    Node *root = NULL;

    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    int i;
    for (i = 0; i < 9; ++i)
        insert(a[i], &root);

    removeNodeByData(5, root);
    print(root, 0);

    printTree(root);
    //traversalLevelorder(root, &printNode); 

    freeTree(&root);
    return 0;
}
