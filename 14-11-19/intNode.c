/*
    Long arithmetic number represened in a singly linked list
        (less to more significant digits)
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include <stdlib.h>
#include "intNode.h"

// writes {digit} to the end of {number} as the new least significant digit
void intNode_Add(IntNode **number, int digit) {    
    IntNode *temp = (IntNode*)malloc(sizeof(IntNode));
    if (temp == NULL) {
        printf("Error: out of memory!\n");
        return;
    }
    temp->val = digit;
    temp->next = *number;
    *number = temp;
}

// releases memory held by {number}
void intNode_Dispose(IntNode **number) {
    while (*number != NULL) {
        intNode_Pop(number);
    }
    free(*number);
}

// creates an empty IntNode
IntNode* intNode_Init() {
    return NULL; // this function is only for incapsulation purposes
}

// removes the least significant digit of {number}
void intNode_Pop(IntNode **number) {
    if (*number != NULL) {
        IntNode *rest = (*number)->next;
        free(*number);
        *number = rest;
    }
}

// prints {revertedNumber} to stdio as decimal integer (NB: number must actually be reverted)
void intNode_PrintRev(IntNode **revertedNumber) {
    IntNode *cur = *revertedNumber;
    while (cur != NULL) {
        printf("%d", cur->val);
        cur = cur->next;
    }
    printf("\n");
}

// creates new IntNode that is equal to {number} reverted as a list
IntNode* intNode_Revert(IntNode **number) {
    IntNode *revertedNumber = intNode_Init(), *cur = *number;
    while (cur != NULL) {
        intNode_Add(&revertedNumber, cur->val);
        cur = cur->next;    
    }
    return revertedNumber;
}
