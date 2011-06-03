#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

struct Stack *createStack()
{
    struct Stack *s = (struct Stack *)malloc(sizeof(struct Stack));
    if (!s) {
        fprintf(stderr, "Failed to allocate memory for a stack\n");
        return NULL;
    }

    s->head = NULL;
    s->size = 0;

    return s;
}

void pushStack(struct Stack *stack, struct BinNode *node)
{
    if (stack == NULL || node == NULL)
        return;

    struct StackItem *n = (struct StackItem *)malloc(sizeof(struct StackItem));
    if (!n) {
        fprintf(stderr, "Failed to allocate memory for the stack node\n");
        return;
    }
    n->node = node;
    
    struct StackItem *head = stack->head;
    n->next = head;
    stack->head = n;
    stack->size++;
}

struct BinNode *popStack(struct Stack *stack)
{
    if (stack->size == 0 || !stack->head)
        return NULL;

    struct StackItem *head = stack->head;
    struct BinNode *n = head->node;
    stack->head = stack->head->next;
    free(head);
    stack->size--;

    return n;
}

int isEmptyStack(struct Stack *stack)
{
    return !stack || stack->size == 0;
}

void clearStack(struct Stack *stack)
{
    if (!stack || !stack->head)
        return;

    struct StackItem *s = stack->head;
    struct StackItem *n;
    while (s != NULL) {
        n = s->next;
        free(s);
        s = n;
    }
    stack->size = 0;
    stack->head = NULL;
}

void freeStack(struct Stack **stack)
{
    if (stack == NULL || *stack == NULL)
        return;
    clearStack(*stack);
    free(*stack);
    *stack = NULL;
}
