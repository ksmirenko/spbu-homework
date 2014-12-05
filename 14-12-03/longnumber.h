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

// clears {lnum} to initial state (*digits == empty SList, *sign == 0)
void        longNumber_Clear(LongNumber *lnum);

// deletes leading (most significant) zeroes in {digits}
void        longNumber_DigitsDeleteLeadingZeroes(SList *digits);

// returns 1 if {digits1} is less than {digits2} and 0 otherwise
int         longNumber_DigitsIsLess(SList *digits1, SList *digits2);

// writes the (not reverted) value of ({digits1} * {digits2}) to {result}
void        longNumber_DigitsMultLongLong(SList *digits1, SList *digits2, SList *result);

// writes the (not reverted) value of ({digits} * {num}) to {result}
void        longNumber_DigitsMultLongShort(SList *digits, int num, SList **result);

// writes the reverted value of abs({digits1} - {digits2}) to {result}
void        longNumber_DigitsSub(SList *digits1, SList *digits2, SList *result);

// writes the reverted value of ({digits1} + {digits2}) to {result}
void        longNumber_DigitsSum(SList *digits1, SList *digits2, SList *result);

// changes the sign of {lnum} to opposite
void        longNumber_DoNeg(LongNumber *lnum);

// releases memory held by {lnum}
void        longNumber_Dispose(LongNumber *lnum);

// creates and returns an empty LongNumber
LongNumber* longNumber_Init();

// writes the product of {lnum1} and {lnum2} to {result}
void        longNumber_Mult(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result);

// prints {lnum} to stdio as decimal integer
void        longNumber_Print(LongNumber *lnum);

// writes the difference between {lnum1} and {lnum2} to {result}
void        longNumber_Sub(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result);

// writes the sum of {lnum1} and {lnum2} to {result}
void        longNumber_Sum(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result);

#endif
