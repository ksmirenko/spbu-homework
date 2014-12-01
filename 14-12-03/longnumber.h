/*
    Long arithmetics library
    Author: Kirill Smirenko, group 171
*/

#ifndef LONGNUM_H
#define LONGNUM_H

#include "slist.h"

extern const int LONG_NUMBER_BASE; // base of the used numeric system

// a long arithmetic number
typedef struct _LongNumber {
    SList *digits; // less to more significant digits
    int *sign; // 0 - positive, 1 - negative
} LongNumber;

// clears {number} to initial state (*digits == empty SList, *sign == 0)
void        longNumber_Clear(LongNumber *lnum);

// returns 1 if {num1} is less than {num2} and 0 otherwise
int         longNumber_DigitsIsLess(SList *digits1, SList *digits2);

// writes the reverted value of abs({num1} - {num2}) to {result}
void        longNumber_DigitsSub(SList *digits1, SList *digits2, SList *result);

// writes the reverted value of ({num1} + {num2}) to {result}
void        longNumber_DigitsSum(SList *digits1, SList *digits2, SList *result);

// changes the sign of {num} to opposite
void        longNumber_DoNeg(LongNumber *lnum);

// releases memory held by {number}
void        longNumber_Dispose(LongNumber *lnum);

// creates and returns an empty LongNumber
LongNumber* longNumber_Init();

// prints {number} to stdio as decimal integer
void        longNumber_Print(LongNumber *lnum);

// writes the difference between {num1} and {num2} to {result}
void        longNumber_Sub(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result);

// writes the sum of {num1} and {num2} to {result}
void        longNumber_Sum(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result);

#endif
