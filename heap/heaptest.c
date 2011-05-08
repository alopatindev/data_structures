#include <stdio.h>
#include "heap.h"

int main()
{
    struct Heap *heap = createHeap();
    int i;
    //int a[] = {3, 5, 1, 4, 2, 7, 0, 8, 9};
    int a[] = {33, 5, 1, 4, 20, 17, 31, 35, 99};
    for (i = 0; i < 9; ++i) {
        char tmp[3] = "";
        snprintf(tmp, 3, "%d", a[i]);
        insertHNode(heap, tmp, &i);
    }
    traversalPreorder(heap->root, &printHNode);
    printf("===\n");
    traversalLevelorder2(heap->root, &printHNode);
    freeHeap(&heap);

    return 0;
}
