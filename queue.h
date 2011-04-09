#ifndef QUEUE_H
#define QUEUE_H

#include "bintree.h"

#define QUEUE_DATA_TYPE BinNode*
//#define QUEUE_DATA_TYPE int

typedef struct QueueNodeStruct
{
    QUEUE_DATA_TYPE data;
    struct QueueNodeStruct *next;
} QueueNode;

typedef struct QueueStruct
{
    QueueNode *head;
    QueueNode *tail;
    size_t size;
} Queue;

Queue *createQueue();
void clearQueue(Queue *queue);
void freeQueue(Queue **queue);
void pushQueue(Queue *q, QUEUE_DATA_TYPE data);
QUEUE_DATA_TYPE popQueue(Queue *q);
void printQueue(Queue *q);
int isEmptyQueue(Queue *q);

#endif
