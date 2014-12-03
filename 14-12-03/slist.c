/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"

// adds {newValue} to the head of {list}
void sList_Add(SList *list, void *newValue) {
	assert(list != NULL);

    // creating new node
    SListNode *temp = (SListNode *)malloc(sizeof(SListNode));
    if (temp == NULL) {
        fprintf(stderr, "Error: not enough memory to create an SListNode!\n");
        return;
    }
	assert(temp != NULL);

    // creating and writing the node's value
    temp->val = malloc(sizeof(list->nodeSize));
    if (temp->val == NULL) {
        fprintf(stderr, "Error: not enough memory to write a SListNode's value!\n");
        return;
    }
    assert(temp->val != NULL);
    memcpy(temp->val, newValue, list->nodeSize);
    
    temp->next = list->head;
    list->head = temp;
}

// removes all nodes of {list}
void sList_Clear(SList *list) {
    assert(list != NULL);
    SListNode *cur;
	while (list->head != NULL) {
		cur = list->head;
		list->head = list->head->next;
		// freeing cur
		if (list->freeFunc) {
			list->freeFunc(cur->val);
		}
		free(cur);
        fprintf(stderr, "freed one\n");
	}
}

// creates and returns deep copy of {listFrom}
SList* sList_Copy(SList *listFrom) {
    assert(listFrom != NULL);
    SList *listTo = sList_Init(listFrom->nodeSize, listFrom->freeFunc);
    SListNode *curNode = listFrom->head;
    while (curNode != NULL) {
        sList_Add(listTo, curNode->val);
        curNode = curNode->next;
    }
    sList_Revert(listTo);
    return listTo;
}

// writes a deep copy of {listFrom} to {listTo}
void  sList_CopyTo(SList *listFrom, SList *listTo) {
    assert(listFrom != NULL);
    sList_Dispose(listTo);
    listTo = sList_Init(listFrom->nodeSize, listFrom->freeFunc);
    SListNode *curNode = listFrom->head;
    while (curNode != NULL) {
        sList_Add(listTo, curNode->val);
        curNode = curNode->next;
    }
    sList_Revert(listTo);
}

// releases system resources held by {list}
void sList_Dispose(SList *list) {
    if (list == NULL) {
        return;
    }
	// freeing nodes
	sList_Clear(list);
    fprintf(stderr, "CLEANED\n");
	// freeing list
	free(list);
}

// invokes {function} for each element of {list} (starting from head)
void sList_Foreach(SList *list, FunctionVoidPvoid function) {
    assert(list != NULL);    
    assert(function != NULL);
    SListNode *curNode = list->head;
    while (curNode != NULL) {
        function(curNode->val);
        curNode = curNode->next;
    }
}

// initializes and returns a new empty list of node size {nodeSize} which uses {freeFunction} to free nodes
SList* sList_Init(int nodeSize, FunctionVoidPvoid freeFunction) {
	assert(nodeSize > 0);
	SList *temp = (SList*)malloc(sizeof(SList));
    if (temp == NULL) {
        fprintf(stderr, "Error: not enough memory to create an SList!\n");
        return NULL;
    }
	assert(temp != NULL);
	temp->nodeSize = nodeSize;
    temp->head = NULL;
    temp->freeFunc = freeFunction;
    return temp;
}

// writes the value of the head node of {list} to {retValue} and then removes the head node
void sList_Remove(SList *list, void *retValue) {
	assert(list != NULL);
	SListNode *curHead = list->head;
	
	if (curHead == NULL) {
		retValue = NULL;
		return;
	}
	
	memcpy(retValue, curHead->val, list->nodeSize);
	list->head = curHead->next;
	// freeing curHead
	if(list->freeFunc) {
		list->freeFunc(curHead->val);
	}
	//free(curHead->val);
	free(curHead);
}

// removes the first occurence of {value} in {list}
void sList_RemoveFirstOcc(SList *list, void *value) {
	assert(list != NULL);
	SListNode *cur = list->head->next, *prev = list->head;
	
	if (!memcmp(list->head->val, value, list->nodeSize)) {
		list->head = list->head->next;
		// freeing prev
		if(list->freeFunc) {
			list->freeFunc(prev->val);
		}
		//free(prev->val);
		free(prev);
		return;
	}
	
	while (cur != NULL) {
		if (!memcmp(cur->val, value, list->nodeSize)) {
            prev->next = cur->next;
			// freeing cur
            if (list->freeFunc) {
				list->freeFunc(cur->val);
			}
			//free(cur->val);
			free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
	}
}

// reverts {list}
void sList_Revert(SList *list) {
    SList *revDigits = sList_Init(list->nodeSize, list->freeFunc);
    SList *trash = list;
    sList_RevertTo(list, revDigits);
    list = revDigits;
    sList_Dispose(trash);
}

// writes the reverted {list} to {result}
void sList_RevertTo(SList *list, SList *result) {
    assert(list != NULL);
    sList_Dispose(result);
    result = sList_Init(list->nodeSize, list->freeFunc);
    SListNode *cur = list->head;
    while (cur != NULL) {
        sList_Add(result, cur->val);
        cur = cur->next;    
    }
}
