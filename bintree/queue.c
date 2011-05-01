#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL) {
        fprintf(stderr, "Failed to allocate memory for a queue\n");
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

void clearQueue(Queue *queue)
{
    if (queue == NULL || queue->size == 0)
        return;

    QueueNode *q = queue->head;
    while (q != NULL) {
        queue->head = queue->head->next;
        free(q);
        q = queue->head;
    }

    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
}

void freeQueue(Queue **queue)
{
    if (queue == NULL || *queue == NULL)
        return;

    clearQueue(*queue);
    free(*queue);
    *queue = NULL;
}

void pushQueue(Queue *q, QUEUE_DATA_TYPE *data)
{
    if (q == NULL || data == NULL)
        return;

    QueueNode *n = (QueueNode *)malloc(sizeof(QueueNode));
    if (n == NULL) {
        fprintf(stderr, "Failed to allocate memory for the queue node\n");
        return;
    }
    n->data = *data;
    n->next = NULL;

    if (q->tail == NULL) {
        q->tail = n;
        q->head = n;
    } else {
        q->tail->next = n;
        q->tail = n;
    }

    q->size++;
}

QUEUE_DATA_TYPE *popQueue(Queue *q)
{
    if (q == NULL || q->head == NULL)
        return NULL;

    QUEUE_DATA_TYPE *data = (QUEUE_DATA_TYPE *)q->head;
    QueueNode *n = q->head->next;

    q->head = n;

    if (q->head == NULL)
        q->tail = NULL;

    q->size--;

    return data;
}

/*void printQueue(Queue *q)
{
    if (q == NULL)
        return;

    QueueNode *n = q->head;
    while (n) {
        //printf("%d ", n->data);
        printf("%d ", n->data);
        n = n->next;
    }
    printf("\n");
}*/

int isEmptyQueue(Queue *q)
{
    return q == NULL || q->size == 0;
}
