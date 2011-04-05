#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

void freeQueue(Queue **queue)
{
    if (queue == NULL)
        return;

    QueueNode *q = (*queue)->head;
    while (q != NULL) {
        (*queue)->head = (*queue)->head->next;
        free(q);
        q = (*queue)->head;
    }
    free(*queue);
    *queue = NULL;
}

void pushQueue(Queue *q, QUEUE_DATA_TYPE data)
{
    if (q == NULL)
        return;

    QueueNode *n = (QueueNode *)malloc(sizeof(QueueNode));
    if (n == NULL) {
        printf("Error: can't allocate memory for the queue");
        return;
    }
    n->data = data;
    n->next = NULL;

    if (q->tail == NULL) {
        q->tail = n;
        q->head = n;
    } else {
        q->tail->next = n;
        q->tail = n;
    }
}

QUEUE_DATA_TYPE popQueue(Queue *q)
{
    if (q == NULL || q->head == NULL)
        return;

    QUEUE_DATA_TYPE data = q->head->data;
    QueueNode *n = q->head->next;
    free(q->head);

    q->head = n;

    if (q->head == NULL)
        q->tail = NULL;

    return data;
}

void printQueue(Queue *q)
{
    if (q == NULL)
        return;

    QueueNode *n = q->head;
    while (n) {
        //printf("%d ", n->data);
        printf("%d ", n->data->data);
        n = n->next;
    }
    printf("\n");
}

int emptyQueue(Queue *q)
{
    //return q->head == q->tail == NULL;
    return q->head == NULL;
}

/*int main()
{
    Queue *q = createQueue();
    pushQueue(q, 1);
    pushQueue(q, 2);
    pushQueue(q, 3);
    printQueue(q);
    printf("first was: %d\n", popQueue(q));
    printQueue(q);
    freeQueue(&q);
    return 0;
}*/
