#ifndef QUEUE_H
#define QUEUE_H

#include "bintree.h"
#include "linkedlist.h"

//#define QUEUE_DATA_TYPE BinNode*
////#define QUEUE_DATA_TYPE int

#define QUEUE_DATA_TYPE Pair
LINKED_LIST(QUEUE_DATA_TYPE, QueueNode, StructQueueNode)

typedef struct QueueStruct
{
    QueueNode *head;
    QueueNode *tail;
    int size;
} Queue;

Queue *createQueue();
void clearQueue(Queue *queue);
void freeQueue(Queue **queue);
void pushQueue(Queue *q, QUEUE_DATA_TYPE *data);
QUEUE_DATA_TYPE *popQueue(Queue *q);
//void printQueue(Queue *q);
int isEmptyQueue(Queue *q);

#endif
