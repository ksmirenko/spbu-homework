/*
    Long arithmetics library
    Author: Kirill Smirenko, group 171
*/

#ifndef LONGNUM_H
#define LONGNUM_H

#include "slist.h"

extern const int LONG_NUMBER_BASE; // base of the used numeric system

// a long arithmetic number
typedef struct _Lnum {
    Slist *digits; // less to more significant digits
    int *sign; // 0 - positive, 1 - negative
} Lnum;

// a delegate that is passed to Slist as a CopyFunction
void    lnum_CloneDelegate(void *memTo, void *memFrom);

// clears {lnum} to initial state (*digits == empty Slist, *sign == 0)
void    lnum_Clear(Lnum *lnum);

// writes a deep copy of {lnumFrom} to {lnumTo}
void    lnum_CopyTo(Lnum *lnumFrom, Lnum *lnumTo);

// adds {digit} to {lnum} as a new least significant digit
void    lnum_DigitAdd(Lnum *lnum, int digit);

// deletes leading (most significant) zeroes in {digits}
void    lnum_DigitsDeleteLeadingZeroes(Slist *digits);

// returns 1 if {digits1} is less than {digits2} and 0 otherwise
int     lnum_DigitsIsLess(Slist *digits1, Slist *digits2);

// writes the (not reverted) value of ({digits1} * {digits2}) to {result}
void    lnum_DigitsMultLongLong(Slist *digits1, Slist *digits2, Slist **result);

// writes the (not reverted) value of ({digits} * {num}) to {result}
void    lnum_DigitsMultLongShort(Slist *digits, int num, Slist **result);

// writes the reverted value of abs({digits1} - {digits2}) to {result}
void    lnum_DigitsSub(Slist *digits1, Slist *digits2, Slist *result);

// writes the reverted value of ({digits1} + {digits2}) to {result}
void    lnum_DigitsSum(Slist *digits1, Slist *digits2, Slist *result);

// writes the result of integer division {lnum1} / {lnum2} to {result}
void    lnum_Div(Lnum *lnum1, Lnum *lnum2, Lnum *result);

// releases memory held by {lnum}
void    lnum_Dispose(Lnum *lnum);

// a delegate that is passed to Slist as a FreeFunction
void    lnum_DisposeDelegate(void *lnum);

// changes the sign of {lnum} to opposite
void    lnum_DoNeg(Lnum *lnum);

// creates and returns an empty Lnum
Lnum*   lnum_Init();

// writes the product of {lnum1} and {lnum2} to {result}
void    lnum_Mult(Lnum *lnum1, Lnum *lnum2, Lnum *result);

// prints {lnum} to stdout as decimal integer
void    lnum_Print(Lnum *lnum);

// prints {lnum} to stderr as decimal integer
void    lnum_PrintDebug(Lnum *lnum);

// writes the difference between {lnum1} and {lnum2} to {result}
void    lnum_Sub(Lnum *lnum1, Lnum *lnum2, Lnum *result);

// writes the sum of {lnum1} and {lnum2} to {result}
void    lnum_Sum(Lnum *lnum1, Lnum *lnum2, Lnum *result);

#endif
