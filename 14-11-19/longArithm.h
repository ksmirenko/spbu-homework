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

// writes the reverted value of abs({num1} - {num2}) to {result}
void        intNode_AbsDiff(IntNode **num1, IntNode **num2, IntNode **result);

// returns 1 if {num1} is less than {num2} and 0 otherwise
int         intNode_IsLess(IntNode **num1, IntNode **num2);

// writes the reverted value of ({num1} + {num2}) to {result}
void        intNode_Sum(IntNode **num1, IntNode **num2, IntNode **result);

// writes the sum of {num1} and {num2} to {result} (digits reverted)
void        longNumber_Add(LongNumber **num1, LongNumber **num2, LongNumber **result);

// clears {number} to initial state
void        longNumber_Clear(LongNumber **number);

// releases memory held by {number}
void        longNumber_Dispose(LongNumber **number);

// changes the sign of {num} to opposite
void        longNumber_DoNeg(LongNumber **num);

// creates an empty LongNumber
LongNumber* longNumber_Init();

// prints {number} to stdio as decimal integer
void        longNumber_Print(LongNumber **number);

// writes the difference between {num1} and {num2} to {result} (digits reverted)
void        longNumber_Sub(LongNumber **num1, LongNumber **num2, LongNumber **result);

#endif
