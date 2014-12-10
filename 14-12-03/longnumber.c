#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"
#include "longnumber.h"

const int LONG_NUMBER_BASE = 10; // base of the used numeric system

// a private delegate that is passed to sList_Foreach; prints one digit to stdout
void printDigit(void *elem) {
    printf("%d", *(int*)(elem));
}

// a delegate that is passed to SList as a CopyFunction
void longNumber_CloneDelegate(void *memTo, void *memFrom) {
    // initializing LongNumber at memTo
    *(LongNumber**)memTo = longNumber_Init();

    LongNumber *lnumFrom = *(LongNumber**)memFrom;
    LongNumber *lnumTo = *(LongNumber**)memTo;
    
    assert(lnumTo != NULL);
    printf("-------------------OK 0\n");
    *lnumTo->sign = *lnumFrom->sign;
    printf("-------------------OK 1\n");
    sList_CopyTo(lnumFrom->digits, &lnumTo->digits); // QUE: will it work?

    printf("Copied to number: {");
    longNumber_Print(lnumTo);
    printf("}\n");
}

// clears {lnum} to initial state (*digits == empty SList, *sign == 0)
void longNumber_Clear(LongNumber *lnum) {
	assert(lnum != NULL);
    sList_Clear(lnum->digits);
    *lnum->sign = 0;
}

// adds {digit} to {lnum} as a new least significant digit
void longNumber_DigitAdd(LongNumber *lnum, int digit) {
    assert(lnum != NULL);
    int buf = digit;
    sList_Add(lnum->digits, (void*)&buf, NULL);
}

// deletes leading (most significant) zeroes in {digits}
void longNumber_DigitsDeleteLeadingZeroes(SList *digits) {
    assert(digits != NULL);
    SListNode *curNode = digits->head->next;
    int lastNonZeroIndex = 0, i = 1;
    while (curNode != NULL) {
        if (*(int*)curNode->val) {
            lastNonZeroIndex = i;
        }
        ++i;
        curNode = curNode->next;
    }
    curNode = digits->head->next;
    SListNode *prevNode = digits->head;
    i = 1;
    while ((curNode != NULL) && (i <= lastNonZeroIndex)) {
        ++i;
        prevNode = curNode;
        curNode = curNode->next;
    }
    while (curNode != NULL) {
        prevNode->next = curNode->next;
		// freeing curNode
        if (digits->freeFunc) {
			digits->freeFunc(curNode->val);
		}
		free(curNode);
        curNode = prevNode->next;
    }
}

// returns 1 if {digits1} is less than {digits2} and 0 otherwise
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

// writes the (not reverted) value of ({digits1} * {digits2}) to {result}
void longNumber_DigitsMultLongLong(SList *digits1, SList *digits2, SList **result) {
    assert(digits1 != NULL);
    assert(digits2 != NULL);

    sList_Clear(*result);
    int zero = 0;
    sList_Add(*result, (void*)&zero, NULL);
    SList *localResult = sList_Init(digits1->nodeSize, digits1->freeFunc);
    SList *temp = sList_Init(digits1->nodeSize, digits1->freeFunc);

    SListNode *curNode2 = digits2->head;
    int shift = 0, i;
    while (curNode2 != NULL) {
        longNumber_DigitsMultLongShort(digits1, *(int*)curNode2->val, &localResult);
        for (i = 0; i < shift; ++i) {
            sList_Add(localResult, (void*)&zero, NULL);
        }
        sList_CopyTo(*result, &temp); 
        longNumber_DigitsSum(temp, localResult, *result);
        sList_Revert(result);

        ++shift;
        curNode2 = curNode2->next;
    }

    sList_Dispose(localResult);
    sList_Dispose(temp);
}

// writes the (not reverted) value of ({digits} * {num}) to {result}
void longNumber_DigitsMultLongShort(SList *digits, int num, SList **result) {
    sList_Clear(*result);
    int carry = 0, buf;
    SListNode *curDigit = digits->head;
    while (curDigit != NULL) {
        carry += *(int*)curDigit->val * num;
        buf = carry % LONG_NUMBER_BASE;
        sList_Add(*result, (void*)&buf, NULL);
        carry /= LONG_NUMBER_BASE;
        curDigit = curDigit->next;
    }
    if (carry) {
        sList_Add(*result, (void*)&carry, NULL);
    }
    sList_Revert(result);
}

// writes the reverted value of abs({digits1} - {digits2}) to {result}
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
            sList_Add(result, (void*)&temp, NULL);
        }        
        curDigit /= LONG_NUMBER_BASE;
    }
    // if result is NULL, then it equals to zero; writing 0
    if (result->head == NULL) {
        int temp = 0;
        sList_Add(result, (void*)&temp, NULL);
    }
    // deleting leading zeroes (they are in the head for {result} is reverted)
    SListNode *curHead = result->head;
    while ((result->head->next != NULL) && !(*(int*)result->head->val)) {
        curHead = result->head;
        result->head = result->head->next;
        // freeing curHead
	    if(result->freeFunc) {
		    result->freeFunc(curHead->val);
	    }
	    free(curHead);
    }
}

// writes the reverted value of ({digits1} + {digits2}) to {result}
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
        sList_Add(result, (void*)&temp, NULL);
        curSum /= LONG_NUMBER_BASE;
    }
    if (curSum) {
        temp = curSum % LONG_NUMBER_BASE;
        sList_Add(result, (void*)&temp, NULL);
    }
}

// writes the result of integer division {lnum1} / {lnum2} to {result}
void longNumber_Div(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
    assert(lnum1 != NULL);
    assert(lnum2 != NULL);
    sList_Clear(result->digits);
    // checking division by zero
    if ((lnum2->digits != NULL)
    && (*(int*)lnum2->digits->head == 0)
    && (lnum2->digits->head->next == NULL)) {
        *result->sign = 0;
        int zero = 0;
        sList_Add(result->digits, (void*)&zero, NULL);
        return;
    }
    // QUE: -2 / 5 == 0 or -2 / 5 == -1 ???
    if (longNumber_DigitsIsLess(lnum1->digits, lnum2->digits)) {
        *result->sign = 0;
        int zero = 0;
        sList_Add(result->digits, (void*)&zero, NULL);
        return;
    }
    // QUE: what is the result of division by negative or by a bigger number? now it's done in a lazy way
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    
    // initializing temp vars
    SList *divisibleReverted = sList_Init(lnum1->digits->nodeSize, lnum1->digits->freeFunc);
    SList *buf = sList_Init(divisibleReverted->nodeSize, divisibleReverted->freeFunc);
    SList *temp = sList_Init(divisibleReverted->nodeSize, divisibleReverted->freeFunc);

    sList_RevertTo(lnum1->digits, divisibleReverted);

    SListNode *curNode = divisibleReverted->head;
    int subCount;
    while (curNode != NULL) {

        while ((buf->head == NULL) || (longNumber_DigitsIsLess(buf, lnum2->digits))) {
            sList_Add(buf, curNode->val, NULL);
            curNode = curNode->next;
        }

        subCount = 0;
        while (!longNumber_DigitsIsLess(buf, lnum2->digits)) {
            // QUE: how do I make this orthodoxial? There are too many memory operations
            sList_CopyTo(buf, &temp);

            longNumber_DigitsSub(temp, lnum2->digits, buf);
            sList_Revert(&buf);
            subCount++;
        }

        sList_Add(result->digits, (void*)&subCount, NULL);
    }

    // freeing memory
    sList_Dispose(temp);
    sList_Dispose(buf);
    sList_Dispose(divisibleReverted);
}

// releases memory held by {lnum}
void longNumber_Dispose(LongNumber *lnum) {
    if (lnum == NULL) {
        return;
    }
    sList_Dispose(lnum->digits);
    free(lnum->sign);
    free(lnum);
}

// a delegate that is passed to SList as a FreeFunction
void longNumber_DisposeDelegate(void *lnum) {
    assert(lnum != NULL);
    longNumber_Dispose((LongNumber*)lnum);
}

// changes the sign of {lnum} to opposite
void longNumber_DoNeg(LongNumber *lnum) {
	*lnum->sign = !(*lnum->sign);
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

// writes the product of {lnum1} and {lnum2} to {result}
void longNumber_Mult(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
    longNumber_Clear(result);
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    longNumber_DigitsMultLongLong(lnum1->digits, lnum2->digits, &result->digits);
}

// prints {lnum} to stdio as decimal integer
void longNumber_Print(LongNumber *lnum) {
    assert(lnum->sign != NULL);
    assert(lnum->digits != NULL);
    if (*lnum->sign) {
        printf("-");
    }
    SList *revDigits = sList_Init(lnum->digits->nodeSize, lnum->digits->freeFunc);
    sList_RevertTo(lnum->digits, revDigits);
    sList_Foreach(revDigits, printDigit);
    sList_Dispose(revDigits);
}

// writes the difference between {lnum1} and {lnum2} to {result}
void longNumber_Sub(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
	longNumber_DoNeg(lnum2);
    longNumber_Sum(lnum1, lnum2, result);
    longNumber_DoNeg(lnum2);
}

// writes the sum of {lnum1} and {lnum2} to {result}
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
    sList_Revert(&result->digits);
}
