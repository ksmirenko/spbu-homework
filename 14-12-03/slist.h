/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#ifndef SLIST_H
#define SLIST_H

#include <stdlib.h>

// a common function that returns void and takes void*
// such functions are used in Slist to free nodes
typedef void (*FunctionVoidPvoid)(void*);

// a common function that returns void and takes void*, void*
// such functions are used in Slist to add new nodes (to do deep copies)
typedef void (*FunctionVoidPvoidPvoid)(void*, void*);

// a single list node
typedef struct _SlistNode {
	// pointer to the value
    void *val;
	// pointer to the next node in the list
    struct _SlistNode *next;
} SlistNode;

// a singly linked list
typedef struct {
	// size of one node (we need it cause we can't tell what we store in the list)
	size_t nodeSize;
	// head node of the list
	SlistNode *head;
	// function that is used to copy node data
	FunctionVoidPvoidPvoid copyFunc;
	// function that is used to free nodes
	FunctionVoidPvoid freeFunc;
} Slist;

// adds {newValue} to the head of {list}
void    slist_Add(Slist *list, void *newValue);

// removes all nodes of {list}
void    slist_Clear(Slist *list);

// writes a deep copy of {listFrom} to {listTo}
void    slist_CopyTo(Slist *listFrom, Slist **listTo);

// releases system resources held by {list}
void	slist_Dispose(Slist *list);

// invokes {function} for each element of {list} (starting from head)
void    slist_Foreach(Slist *list, FunctionVoidPvoid function);

// initializes and returns a new empty list of node size {nodeSize} which uses {copyFunction} to copy nodes and {freeFunction} to free nodes
Slist*  slist_Init(int nodeSize, FunctionVoidPvoidPvoid copyFunction, FunctionVoidPvoid freeFunction);

// writes the value of the head node of {list} to {retValue} and then removes the head node
void	slist_Remove(Slist *list, void *retValue);

// removes the first occurence of {value} in {list}
void	slist_RemoveFirstOcc(Slist *list, void *value);

// reverts {list}
void    slist_Revert(Slist **list);

// writes the reverted {list} to {result}
void    slist_RevertTo(Slist *list, Slist *result);

#endif
