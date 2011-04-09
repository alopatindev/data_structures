#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *createStack()
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (s == NULL) {
        fprintf(stderr, "Failed to allocate memory for a stack\n");
        return NULL;
    }

    s->head = NULL;
    s->size = 0;

    return s;
}

void pushStack(Stack *stack, STACK_DATA_TYPE data)
{
    StackNode *n = (StackNode *)malloc(sizeof(StackNode));
    if (n == NULL) {
        fprintf(stderr, "Failed to allocate memory for the stack node\n");
        return;
    }
    n->data = data;
    
    StackNode *head = stack->head;
    n->next = head;
    stack->head = n;
    ++(stack->size);
}

STACK_DATA_TYPE *popStack(Stack *stack)
{
    if (stack->size == 0 || stack->head == NULL)
        return NULL;

    //STACK_DATA_TYPE data = stack->head->data;
    STACK_DATA_TYPE *head = (STACK_DATA_TYPE *)stack->head;
    stack->head = stack->head->next;
    //free(head);
    --(stack->size);

    //return data;
    return head;
}

/*void printStack(Stack *stack)
{
    if (stack == NULL || stack->head == NULL || stack->size == 0)
        return;

    StackNode *s = stack->head;
    while (s != NULL) {
        printf("%d ", s->data);
        s = s->next;
    }
    printf("\n");
}*/

int isEmptyStack(Stack *stack)
{
    return stack->size == 0;
}

void clearStack(Stack *stack)
{
    if (stack == NULL || stack->head == NULL)
        return;

    StackNode *s = stack->head;
    while (s != NULL) {
        free(s);
        s = s->next;
    }
    stack->size = 0;
    stack->head = NULL;
}

void freeStack(Stack **stack)
{
    clearStack(*stack);
    free(*stack);
    *stack = NULL;
}
