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
void        intNode_Add(IntNode **number, int digit);

// clears {number} to NULL state
void        intNode_Clear(IntNode **number);

// releases memory held by {number}
void        intNode_Dispose(IntNode **number);

// creates an empty IntNode
IntNode*    intNode_Init();

// removes the least significant digit of {number}
void        intNode_Pop(IntNode **number);

// prints {revertedNumber} to stdio as decimal integer (NB: number must actually be reverted)
void        intNode_PrintRev(IntNode **revertedNumber);

// reverts {number} and writes it to {result}
void    intNode_Revert(IntNode **number, IntNode **result);

#endif
