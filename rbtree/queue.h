#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

#define LIST List
#define QUEUE(QUEUE_NAME, DATA_TYPE) \
LINKED_LIST(LIST##QUEUE_NAME, DATA_TYPE) \
struct QUEUE_NAME \
{ \
    struct LIST##QUEUE_NAME *head; \
    struct LIST##QUEUE_NAME *tail; \
    int size; \
}; \
\
struct QUEUE_NAME *create##QUEUE_NAME() \
{ \
    struct QUEUE_NAME *q = (struct QUEUE_NAME *)\
                           malloc(sizeof(struct QUEUE_NAME)); \
    if (!q) { \
        fprintf(stderr, "Failed to allocate memory for a queue\n"); \
        return NULL; \
    } \
    q->head = NULL; \
    q->tail = NULL; \
    q->size = 0; \
    return q; \
} \
\
void clear##QUEUE_NAME(struct QUEUE_NAME *queue) \
{ \
    if (!queue || queue->size == 0) \
        return; \
    struct LIST##QUEUE_NAME *q = queue->head; \
    while (q) { \
        queue->head = queue->head->next; \
        free(q); \
        q = queue->head; \
    } \
} \
\
void free##QUEUE_NAME(struct QUEUE_NAME **queue) \
{ \
    clear##QUEUE_NAME(*queue); \
    free(*queue); \
    *queue = NULL; \
} \
\
void push##QUEUE_NAME(struct QUEUE_NAME *q, DATA_TYPE *data) \
{ \
    if (!q) \
        return; \
    struct LIST##QUEUE_NAME *n = (struct LIST##QUEUE_NAME *) \
                                 malloc(sizeof(struct LIST##QUEUE_NAME)); \
    if (!n) { \
        fprintf(stderr, "Failed to allocate memory for the queue node\n"); \
        return; \
    } \
    n->data = *data; \
    n->next = NULL; \
    if (!(q->tail)) { \
        q->tail = n; \
        q->head = n; \
    } else { \
        q->tail->next = n; \
        q->tail = n; \
    } \
    q->size++; \
} \
\
struct LIST##QUEUE_NAME *pop##QUEUE_NAME(struct QUEUE_NAME *q) \
{ \
    if (!q || !(q->head)) \
        return NULL; \
    struct LIST##QUEUE_NAME *head = q->head; \
    struct LIST##QUEUE_NAME *n = q->head->next; \
    q->head = n; \
    if (!(q->head)) \
        q->tail = NULL; \
    q->size--; \
    return head; \
} \
DATA_TYPE *top##QUEUE_NAME(struct QUEUE_NAME *q) \
{ \
    if (!q || !(q->head)) \
        return NULL; \
    return &(q->head->data); \
} \
\
int isEmpty##QUEUE_NAME(struct QUEUE_NAME *q) \
{ \
    return q->size == 0; \
} \
\
void print##QUEUE_NAME(struct QUEUE_NAME *q) \
{ \
    if (!q) \
        return; \
    struct LIST##QUEUE_NAME *n = q->head; \
    while (n) { \
        printf("%d ", n->data); \
        n = n->next; \
    } \
    printf("\n"); \
}

#endif
