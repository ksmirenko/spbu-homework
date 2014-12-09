/*
    Generic stack
    Author: Kirill Smirenko, group 171
*/

#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include "slist.h"

// a singly linked list
typedef struct {
    // current size of the stack
    int *size;
    // SList where the stack content is actually stored
    SList *list;
} Stack;

// clears {stack}
void    stack_Clear(Stack *stack);

// writes a deep copy of {stackFrom} to {stackTo}
void    stack_CopyTo(Stack *stackFrom, Stack *stackTo);

// releases system resources held by {stack}
void	stack_Dispose(Stack *stack);

// initializes and returns a new empty list of node size {nodeSize} which uses {freeFunction} to free nodes
Stack*  stack_Init(int nodeSize, FunctionVoidPvoid freeFunction);

// writes the value of the top element of {stack} to {retValue} and then removes the top element
void	stack_Pop(Stack *stack, void *retValue);

// puts {newValue} to {stack} using {copyFunc}
void    stack_Push(Stack *stack, void *newValue, FunctionVoidPvoidPvoid copyFunc);

// writes the value of the top element of {stack} to {retValue}
void	stack_Top(Stack *stack, void *retValue);

#endif
