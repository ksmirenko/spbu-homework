/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#ifndef SLIST_H
#define SLIST_H

// a common function that returns void and takes void*
// such functions are used here to free list nodes
typedef void (*FunctionVoidPvoid)(void*);

// a single list node
typedef struct _SListNode {
	// pointer to the value
    void *val;
	// pointer to the next node in the list
    struct _SListNode *next;
} SListNode;

// a singly linked list
typedef struct {
	// size of one node (we need it cause we can't tell what we store in the list)
	size_t nodeSize;
	// head node of the list
	SListNode *head;
	// function that is used to free nodes
	FunctionVoidPvoid freeFunc;
} SList;

// adds {newValue} to the head of {list}
void	sList_Add(SList *list, void *newValue);

// releases system resources held by {list}
void	sList_Dispose(SList *list);

// initializes and returns a new empty list which uses {freeFunction} as a memory freeing function
SList*	sList_Init(FunctionVoidPvoid freeFunction);

// prints all SListNodes of {list} (starting from head, to stdio, divided by space)
void	sList_Print(SList *list);

// writes the value of the head node of {list} to {retValue} and then removes the head node
void	sList_Remove(SList *list, void *retValue);

// removes the first occurence of {value} in {list}
void	sList_RemoveFirstOcc(SList *list, void *value);

#endif
