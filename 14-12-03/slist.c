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
void slist_Add(Slist *list, void *newValue) {
	assert(list != NULL);

    // creating new node
    SlistNode *newNode = (SlistNode *)malloc(sizeof(SlistNode));
    if (newNode == NULL) {
        fprintf(stderr, "Error: not enough memory to create an SlistNode!\n");
        return;
    }
	assert(newNode != NULL);

    // creating and writing the node's value
    newNode->val = malloc(sizeof(list->nodeSize));
    if (newNode->val == NULL) {
        fprintf(stderr, "Error: not enough memory to write a SlistNode's value!\n");
        return;
    }
    assert(newNode->val != NULL);
    
    // copying memory data
    if (list->copyFunc == NULL) {
        // does not need any special function to copy
        memcpy(newNode->val, newValue, list->nodeSize);
    }
    else {
        // needs a special function to copy
        list->copyFunc(newNode->val, newValue);
    }
    
    newNode->next = list->head;
    list->head = newNode;
    
    //assert(0);
}

// removes all nodes of {list}
void slist_Clear(Slist *list) {
    assert(list != NULL);
    SlistNode *cur;
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
void slist_CopyTo(Slist *listFrom, Slist **listTo) {
    assert(listFrom != NULL);
    slist_RevertTo(listFrom, *listTo);
    slist_Revert(listTo);
}

// releases system resources held by {list}
void slist_Dispose(Slist *list) {
    if (list == NULL) {
        return;
    }
	// freeing nodes
	slist_Clear(list);
	// freeing list
	free(list);
}

// invokes {function} for each element of {list} (starting from head)
void slist_Foreach(Slist *list, FunctionVoidPvoid function) {
    assert(list != NULL);    
    assert(function != NULL);
    SlistNode *curNode = list->head;
    while (curNode != NULL) {
        function(curNode->val);
        curNode = curNode->next;
    }
}

// initializes and returns a new empty list of node size {nodeSize} which uses {copyFunction} to copy nodes and {freeFunction} to free nodes
Slist*  slist_Init(int nodeSize, FunctionVoidPvoidPvoid copyFunction, FunctionVoidPvoid freeFunction) {
	assert(nodeSize > 0);
	Slist *temp = (Slist*)malloc(sizeof(Slist));
    if (temp == NULL) {
        fprintf(stderr, "Error: not enough memory to create an Slist!\n");
        return NULL;
    }
	assert(temp != NULL);
	temp->nodeSize = nodeSize;
    temp->head = NULL;
    temp->copyFunc = copyFunction;
    temp->freeFunc = freeFunction;
    return temp;
}

// writes the value of the head node of {list} to {retValue} and then removes the head node
void slist_Remove(Slist *list, void *retValue) {
	assert(list != NULL);
	SlistNode *curHead = list->head;
	
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
void slist_RemoveFirstOcc(Slist *list, void *value) {
	assert(list != NULL);
	SlistNode *cur = list->head->next, *prev = list->head;
	
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
void slist_Revert(Slist **list) {
    Slist *revDigits = slist_Init((*list)->nodeSize, (*list)->copyFunc, (*list)->freeFunc);
    Slist *trash = (*list);
    slist_RevertTo((*list), revDigits);
    (*list) = revDigits;
    slist_Dispose(trash);
}

// writes the reverted {list} to {result}
void slist_RevertTo(Slist *listFrom, Slist *listTo) {
    assert(listFrom != NULL);
    slist_Clear(listTo);
    SlistNode *cur = listFrom->head;
    while (cur != NULL) {
        slist_Add(listTo, cur->val);
        cur = cur->next;    
    }
}
