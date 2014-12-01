#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "slist.h"
#include "longnumber.h"

const int LONG_NUMBER_BASE = 10; // base of the used numeric system

void printDigit(void *elem) {
    printf("%d", *(int*)(elem));
}

// clears {number} to initial state (*digits == empty SList, *sign == 0)
void longNumber_Clear(LongNumber *lnum) {
	assert(lnum != NULL);
    sList_Clear(lnum->digits);
    *lnum->sign = 0;
}

// returns 1 if {num1} is less than {num2} and 0 otherwise
int longNumber_DigitsIsLess(SList *digits1, SList *digits2) {
	SListNode *cur1 = digits1->head, *cur2 = digits2->head;
    int isFirstLess = 0, val1 = 0, val2 = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            val1 = *(int*)cur1->val;
            cur1 = cur1->next;
        }
        else {
            return 1;
        }
        if (cur2 != NULL) {
            val2 = *(int*)cur2->val;
            cur2 = cur2->next;
        }
        else {
            return 0;
        }
        if (val1 != val2) {
            isFirstLess = (val1 < val2);
        }
    }
    return isFirstLess;
}

// writes the reverted value of abs({num1} - {num2}) to {result}
void longNumber_DigitsSub(SList *digits1, SList *digits2, SList *result) {
	// if num1 < num2, return (num2 - num1) instead
    if (longNumber_DigitsIsLess(digits1, digits2)) {
        longNumber_DigitsSub(digits2, digits1, result);
        return;
    }

    sList_Clear(result);
    SListNode *cur1 = digits1->head, *cur2 = digits2->head;
    int curDigit = 0, val1 = 0, val2 = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            val1 += *(int*)cur1->val;
            cur1 = cur1->next;
        }
        if (cur2 != NULL) {
            val2 = *(int*)cur2->val;
            cur2 = cur2->next;
        }
        else {
            val2 = 0;
        }
        if (val1 >= val2) {
            curDigit = val1 - val2;
            val1 = 0;
        }
        else {
            curDigit = (LONG_NUMBER_BASE + val1 - val2) % LONG_NUMBER_BASE;
            val1 = -1;
        }
        if ((curDigit > 0) || (cur1 != NULL)) {
            int temp = curDigit % LONG_NUMBER_BASE;
            sList_Add(result, (void*)&temp);
        }        
        curDigit /= LONG_NUMBER_BASE;
    }
}

// writes the reverted value of ({num1} + {num2}) to {result}
void longNumber_DigitsSum(SList *digits1, SList *digits2, SList *result) {
	sList_Clear(result);
    SListNode *cur1 = digits1->head, *cur2 = digits2->head;
    int curSum = 0, temp = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            curSum += *(int*)cur1->val;
            cur1 = cur1->next;
        }
        if (cur2 != NULL) {
            curSum += *(int*)cur2->val;
            cur2 = cur2->next;
        }
        temp = curSum % LONG_NUMBER_BASE;
        sList_Add(result, (void*)&temp);
        curSum /= LONG_NUMBER_BASE;
    }
    if (curSum) {
        temp = curSum % LONG_NUMBER_BASE;
        sList_Add(result, (void*)&temp);
    }
}

// changes the sign of {num} to opposite
void longNumber_DoNeg(LongNumber *lnum) {
	*lnum->sign = !(*lnum->sign);
}

// releases memory held by {number}
void longNumber_Dispose(LongNumber *lnum) {
	assert(lnum != NULL);
    sList_Dispose(lnum->digits);
    free(lnum->sign);
    free(lnum);
}

// creates and returns an empty LongNumber
LongNumber* longNumber_Init() {
	LongNumber *temp = (LongNumber*)malloc(sizeof(LongNumber));
    if (temp == NULL) {
        fprintf(stderr, "Error: not enough memory to create a LongNumber!\n");
        return NULL;
    }
	assert(temp != NULL);
    temp->digits = sList_Init(sizeof(int), NULL);
    temp->sign = malloc(sizeof(int));
    if (temp->sign == NULL) {
        fprintf(stderr, "Error: not enough memory to create an int!\n");
        return NULL;
    }
	assert(temp->sign != NULL);
    *temp->sign = 0;
    return temp;
}

// prints {number} to stdio as decimal integer
void longNumber_Print(LongNumber *lnum) {
    if (*lnum->sign) {
        printf("-");
    }
    SList *revDigits = sList_Init(lnum->digits->nodeSize, lnum->digits->freeFunc);
    sList_Revert(lnum->digits, revDigits);
    sList_Foreach(revDigits, printDigit);
    sList_Dispose(revDigits);
}

// writes the difference between {num1} and {num2} to {result}
void longNumber_Sub(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
	longNumber_DoNeg(lnum2);
    longNumber_Sum(lnum1, lnum2, result);
    longNumber_DoNeg(lnum2);
}

// writes the sum of {num1} and {num2} to {result}
void longNumber_Sum(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
	longNumber_Clear(result);
    if (*lnum1->sign == *lnum2->sign) {
        *result->sign = *lnum1->sign;
        longNumber_DigitsSum(lnum1->digits, lnum2->digits, result->digits);
    }
    else {
        if (longNumber_DigitsIsLess(lnum1->digits, lnum2->digits)) {
            *result->sign = *lnum2->sign;
        }
        else {
            *result->sign = *lnum1->sign;
        }
        longNumber_DigitsSub(lnum1->digits, lnum2->digits, result->digits);
    }

    // reverting result->digits
    SList *revDigits = sList_Init(result->digits->nodeSize, result->digits->freeFunc);
    SList *trash = result->digits;
    sList_Revert(result->digits, revDigits);
    result->digits = revDigits;
    sList_Dispose(trash);
}
