#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "linkedlist.h"

#define STACK(STACK_NAME, DATA_TYPE) \
LINKED_LIST(LIST##STACK_NAME, DATA_TYPE) \
struct STACK_NAME \
{ \
    struct LIST##STACK_NAME *head; \
    int size; \
}; \
\
struct STACK_NAME *create##STACK_NAME() \
{ \
    struct STACK_NAME *s = (struct STACK_NAME *)malloc(sizeof(struct STACK_NAME)); \
    if (!s) { \
        fprintf(stderr, "Failed to allocate memory for a stack\n"); \
        return NULL; \
    } \
    s->head = NULL; \
    s->size = 0; \
    return s; \
} \
\
void push##STACK_NAME(struct STACK_NAME *stack, DATA_TYPE *data) \
{ \
    if (!stack || !data) \
        return; \
    struct LIST##STACK_NAME *n = (struct LIST##STACK_NAME *)malloc(sizeof(\
        struct LIST##STACK_NAME)); \
    if (!n) { \
        fprintf(stderr, "Failed to allocate memory for the stack node\n"); \
        return; \
    } \
    n->data = *data; \
    struct LIST##STACK_NAME *head = stack->head; \
    n->next = head; \
    stack->head = n; \
    stack->size++; \
} \
\
struct LIST##STACK_NAME *top##STACK_NAME(struct STACK_NAME *stack) \
{ \
    if (!stack || stack->size == 0) \
        return NULL; \
    return stack->head; \
} \
struct LIST##STACK_NAME *pop##STACK_NAME(struct STACK_NAME *stack) \
{ \
    if (!stack || stack->size == 0 || !stack->head) \
        return NULL; \
    struct LIST##STACK_NAME *head = (struct LIST##STACK_NAME *)stack->head; \
    stack->head = stack->head->next; \
    stack->size--; \
    return head; \
} \
\
int isEmpty##STACK_NAME(struct STACK_NAME *stack) \
{ \
    return !stack || stack->size == 0; \
} \
\
void clear##STACK_NAME(struct STACK_NAME *stack) \
{ \
    if (!stack || !stack->head) \
        return; \
    struct LIST##STACK_NAME *s = stack->head; \
    struct LIST##STACK_NAME *n; \
    while (s) { \
        n = s->next; \
        free(s); \
        s = n; \
    } \
    stack->size = 0; \
    stack->head = NULL; \
} \
\
void free##STACK_NAME(struct STACK_NAME **stack) \
{ \
    if (!stack || !*stack) \
        return; \
    clear##STACK_NAME(*stack); \
    free(*stack); \
    *stack = NULL; \
} \
\
void print##STACK_NAME(struct STACK_NAME *stack) \
{ \
    if (!stack || !stack->head || stack->size == 0) \
        return; \
    struct LIST##STACK_NAME *s = stack->head; \
    while (s != NULL) { \
        printf("%d ", s->data); \
        s = s->next; \
    } \
    printf("\n"); \
} \

#endif
