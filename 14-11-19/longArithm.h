/*
    Long arithmetics library
    Author: Kirill Smirenko, group 171
*/

#ifndef LONGARITHM_H
#define LONGARITHM_H

// a long arithmetic number
typedef struct LongNumber {
    int sign; // 0 - positive, 1 - negative
    IntNode *digits;
} LongNumber;

// creates a new IntNode that is equal to reverted value of abs({num1} - {num2})
IntNode* intNode_AbsDiff(IntNode **num1, IntNode **num2);

// returns 1 if {num1} is less than {num2} and 0 otherwise
int intNode_IsLess(IntNode **num1, IntNode **num2);

// creates a new IntNode that is equal to reverted value of ({num1} + {num2})
IntNode* intNode_Sum(IntNode **num1, IntNode **num2);

// creates a new LongNumber that is equal to sum of {num1} and {num2}
LongNumber* longNumber_Add(LongNumber **num1, LongNumber **num2);

// releases memory held by {number}
void longNumber_Dispose(LongNumber **number);

// creates an empty LongNumber
LongNumber* longNumber_Init();

// prints {number} to stdio as decimal integer
void longNumber_Print(LongNumber **number);

// creates a new LongNumber that is equal to difference between {num1} and {num2}
LongNumber* longNumber_Sub(LongNumber **num1, LongNumber **num2);

// changes the sign of {num} to opposite
void longNumber_DoNeg(LongNumber **num);

#endif
