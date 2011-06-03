#ifndef STACK_H
#define STACK_H

#include "bintree.h"
#include <stdlib.h>

struct StackItem
{
    struct BinNode *node;
    struct StackItem *next;
};

struct Stack
{
    struct StackItem *head;
    int size;
};

struct Stack *createStack();
void pushStack(struct Stack *stack, struct BinNode *node);
struct BinNode *popStack(struct Stack *stack);
void clearStack(struct Stack *stack);
void freeStack(struct Stack **stack);
int isEmptyStack(struct Stack *stack);

#endif
