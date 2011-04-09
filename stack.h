#ifndef STACK_H
#define STACK_H

#include "bintree.h"
#include "linkedlist.h"

//#define STACK_DATA_TYPE int
#define STACK_DATA_TYPE BinNode*

LINKED_LIST(STACK_DATA_TYPE, StackNode, StructStackNode)

typedef struct
{
    StackNode *head;
    size_t size;
} Stack;

Stack *createStack();
void pushStack(Stack *stack, STACK_DATA_TYPE data);
STACK_DATA_TYPE popStack(Stack *stack);
//void printStack(Stack *stack);
void clearStack(Stack *stack);
void freeStack(Stack **stack);
int isEmptyStack(Stack *stack);

int main();

#endif
