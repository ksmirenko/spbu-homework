/*
    Long arithmetic number represened in a singly linked list
        (less to more significant digits)
    Author: Kirill Smirenko, group 171
*/

#ifndef INTNODE_H
#define INTNODE_H

// a single digit of a long number; the head
typedef struct IntNode {
    int val;
    struct IntNode *next;
} IntNode;

// writes {digit} to the end of {number} as the new least significant digit
void intNode_Add(IntNode **number, int digit);

// sets {number} to NULL
void intNode_Dispose(IntNode **number);

// initializes an empty IntNode
IntNode* intNode_Init();

// removes the least significant digit of {number}
void intNode_Pop(IntNode **number);

// prints {revertedNumber} to stdio as decimal integer (NB: number must actually be reverted)
void intNode_PrintRev(IntNode **revertedNumber);

// creates new IntNode that is equal to {number} reverted as a list
IntNode* intNode_Revert(IntNode **number);

#endif
