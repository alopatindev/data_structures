#include "bintree.h"
#include <stdlib.h>
#include "queue.h"
#include <stdio.h>
//#include <time.h>

int main()
{
    // deletion example from http://habrahabr.ru/blogs/algorithm/65617/
    Node *root = NULL;

    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    int i;
    for (i = 0; i < 9; ++i)
        insert(a[i], &root);

    printTree(root);
    //removeNodeByData(5, root);
    //print(root, 0);
    printTreeByLevel(root);

    freeTree(&root);
    return 0;
}
