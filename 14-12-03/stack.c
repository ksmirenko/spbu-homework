/*
    Generic stack
    Author: Kirill Smirenko, group 171
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "stack.h"

// clears {stack}
void stack_Clear(Stack *stack) {
    assert(stack != NULL);
    sList_Clear(stack->list);
    *stack->size = 0;
}

// writes a deep copy of {stackFrom} to {stackTo}
void stack_CopyTo(Stack *stackFrom, Stack *stackTo) {
    assert(stackFrom != NULL);
    *stackTo->size = *stackFrom->size;
    sList_CopyTo(stackFrom->list, &stackTo->list);
}

// releases system resources held by {stack}
void stack_Dispose(Stack *stack) {
    if (stack == NULL) {
        return;
    }
    stack_Clear(stack);
    free(stack->size);
    sList_Dispose(stack->list);
    free(stack);
}

// initializes and returns a new empty stack of element size {nodeSize} which uses {freeFunction} to free elements
Stack* stack_Init(int nodeSize, FunctionVoidPvoid freeFunction) {
    Stack *newStack = (Stack*)malloc(sizeof(Stack));
    if (newStack == NULL) {
        fprintf(stderr, "Error: not enough memory to create an Stack!\n");
        return NULL;
    }
	assert(newStack != NULL);
    newStack->size = malloc(sizeof(int));
    if (newStack->size == NULL) {
        fprintf(stderr, "Error: not enough memory to create an int!\n");
        return NULL;
    }
	assert(newStack->size != NULL);
    *newStack->size = 0;
    newStack->list = sList_Init(nodeSize, freeFunction);
    return newStack;
}

// writes the value of the top element of {stack} to {retValue} and then removes the top element
void stack_Pop(Stack *stack, void *retValue) {
    stack_Top(stack, retValue);
    if (stack->size > 0) {
        sList_Remove(stack->list, retValue);
        --*stack->size;
    }
}

// puts {newValue} to {stack}
void stack_Push(Stack *stack, void *newValue) {
    sList_Add(stack->list, newValue);
    ++*stack->size;
}

// writes the value of the top element of {stack} to {retValue}
void stack_Top(Stack *stack, void *retValue) {
    if (stack->size > 0) {
        memcpy(retValue, stack->list->head->val, stack->list->nodeSize);
    }
}
