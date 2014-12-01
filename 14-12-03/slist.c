/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

// adds {newValue} to the head of {list}
void sList_Add(SList *list, void *newValue) {
	assert(list != NULL);
    SListNode *temp = (SListNode *)malloc(sizeof(SListNode));
    if (temp == NULL) {
        printf(stderr, "Error: not enough memory to create an SListNode!\n");
        return;
    }
	assert(temp != NULL);
    temp->val = newValue;
    temp->next = list->head; // NB: (*list) or list?
    list->head = temp; // NB: (*list) or list?
}

// releases system resources held by {list}
void sList_Dispose(SList *list) {
	// freeing nodes
	SListNode *cur;
	while (list->head != NULL) { // NB: (*list) or list?
		cur = list->head; // NB: (*list) or list?
		list->head = list->head->next; // NB: (*list) or list?
		// freeing cur
		if (list->freeFunc) { // NB: (*list) or list?
			list->freeFunc(cur->val); // NB: (*list) or list?
		}
		free(cur->val);
		free(cur);
	}
	
	// freeing list
	free(list);
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
    SListNode *cur = list->head; // NB: (*list) or list?
    while (cur != NULL) {
        printf("%d ", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

// writes the value of the head node of {list} to {retValue} and then removes the head node
void sList_Remove(SList *list, void *retValue) {
	assert(list != NULL);
	SListNode *curHead = list->head; // NB: (*list) or list?
	
	if (curHead == NULL) {
		retValue = NULL;
		return;
	}
	
	memcpy(retValue, curHead->val, list->nodeSize);
	list->head = curHead->next; // NB: (*list) or list?
	// freeing curHead
	if(list->freeFunc) { // NB: (*list) or list?
		list->freeFunc(curHead->val); // NB: (*list) or list?
	}
	free(curHead->val);
	free(curHead);
}

// removes the first occurence of {value} in {list}
void sList_RemoveFirstOcc(SList *list, void *value) {
	assert(list != NULL);
	SListNode *cur = list->head->next, *prev = list->head; // NB: (*list) or list?
	
	if (!memcmp(list->head->val, value, list->nodeSize)) { // NB: (*list) or list?
		list->head = list->head->next; // NB: (*list) or list?
		// freeing prev
		if(list->freeFunc) { // NB: (*list) or list?
			list->freeFunc(prev->val); // NB: (*list) or list?
		}
		free(prev->val);
		free(prev);
		return;
	}
	
	while (cur != NULL) {
		if (!memcmp(cur->val, value, list->nodeSize)) {
            prev->next = cur->next;
			// freeing cur
            if (list->freeFunc) { // NB: (*list) or list?
				list->freeFunc(cur->val); // NB: (*list) or list?
			}
			free(cur->val);
			free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
	}
}