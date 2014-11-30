/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

// adds {newValue} to the head of {list}
void sList_Add(SList *list, void *newValue) {
    SListNode *temp = (SListNode *)malloc(sizeof(SListNode));
    if (temp == NULL) {
        printf(stderr, "Error: not enough memory to create an SListNode!\n");
        return;
    }
	assert(temp != NULL);
    temp->val = newValue;
    temp->next = (*list)->head; // NB: (*list) or list?
    (*list)->head = temp; // NB: (*list) or list?
}

// releases system resources held by {list}
void sList_Dispose(SList *list) {
	// TODO: implement
}

// initializes and returns a new empty list of node size {nodeSize} which uses {freeFunction} to free nodes
SList* sList_Init(int nodeSize, FunctionVoidPvoid freeFunction) {
	assert(nodeSize > 0);
	SList *temp = (SList*)malloc(sizeof(SList));
    if (temp == NULL) {
        printf(stderr, "Error: not enough memory to create an SList!\n");
        return NULL;
    }
	assert(temp != NULL);
	temp->nodeSize = nodeSize;
    temp->head = NULL;
    temp->freeFunc = freeFunction;
    return temp;
}

// prints all SListNodes of {list} (starting from head, to stdio, divided by space)
void sList_Print(SList *list) {
    SListNode *cur = (*list)->head; // NB: (*list) or list?
    while (cur != NULL) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

// writes the value of the head node of {list} to {retValue} and then removes the head node
void sList_Remove(SList *list, void *retValue) {
	SListNode *curHead = (*list)->head; // NB: (*list) or list?
	
	if (curHead == NULL) {
		retValue = NULL;
		return;
	}
	
	memcpy(retValue, curHead->val, list->nodeSize);
	(*list)->head = curHead->next; // NB: (*list) or list?
	
	if((*list)->freeFunc) { // NB: (*list) or list?
		(*list)->freeFunc(curHead->val); // NB: (*list) or list?
	}
	free(curHead->val);
	free(curHead);
}

// removes the first occurence of {value} in {list}
void sList_RemoveFirstOcc(SList *list, void *value) {
	// TODO: implement
}