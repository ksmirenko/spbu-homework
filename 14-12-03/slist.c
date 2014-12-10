/*
    Generic singly linked list
    Author: Kirill Smirenko, group 171
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"

// adds {newValue} to the head of {list} using {copyFunc} to copy data
void sList_Add(SList *list, void *newValue, FunctionVoidPvoidPvoid copyFunc) {
	assert(list != NULL);

    // creating new node
    SListNode *newNode = (SListNode *)malloc(sizeof(SListNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: not enough memory to create an SListNode!\n");
        return;
    }
	assert(newNode != NULL);

    // creating and writing the node's value
    newNode->val = malloc(sizeof(list->nodeSize));
    if (newNode->val == NULL) {
        fprintf(stderr, "Error: not enough memory to write a SListNode's value!\n");
        return;
    }
    assert(newNode->val != NULL);
    
    // copying memory data
    if (copyFunc == NULL) {
        // does not need any special function to copy
        memcpy(newNode->val, newValue, list->nodeSize);
    }
    else {
        // needs a special function to copy
        printf("COPYING\n");
        copyFunc(newNode->val, newValue);
    }
    
    newNode->next = list->head;
    list->head = newNode;
    
    //assert(0);
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
        free(cur->val);
		free(cur);
	}
}

// writes a deep copy of {listFrom} to {listTo}
void sList_CopyTo(SList *listFrom, SList **listTo) {
    assert(listFrom != NULL);
    sList_RevertTo(listFrom, *listTo);
    sList_Revert(listTo);
}

// releases system resources held by {list}
void sList_Dispose(SList *list) {
    if (list == NULL) {
        return;
    }
	// freeing nodes
	sList_Clear(list);
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
	free(curHead->val);
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
		free(prev->val);
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
			free(cur->val);
			free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
	}
}

// reverts {list}
void sList_Revert(SList **list) {
    SList *revDigits = sList_Init((*list)->nodeSize, (*list)->freeFunc);
    SList *trash = (*list);
    sList_RevertTo((*list), revDigits);
    (*list) = revDigits;
    sList_Dispose(trash);
}

// writes the reverted {list} to {result}
void sList_RevertTo(SList *listFrom, SList *listTo) {
    assert(listFrom != NULL);
    sList_Clear(listTo);
    SListNode *cur = listFrom->head;
    while (cur != NULL) {
        sList_Add(listTo, cur->val, NULL);
        cur = cur->next;    
    }
}
