#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "slist.h"
#include "longnumber.h"

const int LONG_NUMBER_BASE = 10; // base of the used numeric system

void printDigit(void *elem) {
    printf("%d", *(int*)(elem));
}

// clears {lnum} to initial state (*digits == empty SList, *sign == 0)
void longNumber_Clear(LongNumber *lnum) {
	assert(lnum != NULL);
    sList_Clear(lnum->digits);
    *lnum->sign = 0;
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
    sList_Add(*result, (void*)&zero);
    SList *localResult = sList_Init(digits1->nodeSize, digits1->freeFunc);
    SList *temp = sList_Init(digits1->nodeSize, digits1->freeFunc);

    SListNode *curNode2 = digits2->head;
    int shift = 0, i;
    while (curNode2 != NULL) {
        longNumber_DigitsMultLongShort(digits1, *(int*)curNode2->val, &localResult);
        for (i = 0; i < shift; ++i) {
            sList_Add(localResult, (void*)&zero);
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
        sList_Add(*result, (void*)&buf);
        carry /= LONG_NUMBER_BASE;
        curDigit = curDigit->next;
    }
    if (carry) {
        sList_Add(*result, (void*)&carry);
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
            sList_Add(result, (void*)&temp);
        }        
        curDigit /= LONG_NUMBER_BASE;
    }
    // if result is NULL, then it equals to zero; writing 0
    if (result->head == NULL) {
        int temp = 0;
        sList_Add(result, (void*)&temp);
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
        sList_Add(result, (void*)&temp);
        curSum /= LONG_NUMBER_BASE;
    }
    if (curSum) {
        temp = curSum % LONG_NUMBER_BASE;
        sList_Add(result, (void*)&temp);
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
        sList_Add(result->digits, (void*)&zero);
        return;
    }
    // QUE: -2 / 5 == 0 or -2 / 5 == -1 ???
    if (longNumber_DigitsIsLess(lnum1->digits, lnum2->digits)) {
        *result->sign = 0;
        int zero = 0;
        sList_Add(result->digits, (void*)&zero);
        return;
    }
    // QUE: what is the result of division by negative or by a bigger number? now it's done in a lazy way
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    
    // initializing temp vars
    SList *divisibleReverted = sList_Init(lnum1->digits->nodeSize, lnum1->digits->freeFunc);
    SList *buf = sList_Init(divisibleReverted->nodeSize, divisibleReverted->freeFunc);
    SList *temp = sList_Init(divisibleReverted->nodeSize, divisibleReverted->freeFunc);

    sList_RevertTo(lnum1->digits, divisibleReverted);
    // TODO: remove debug output
    printf("--------------------divisibleReverted: {");
    sList_Foreach(divisibleReverted, printDigit);
    printf("}\n");

    SListNode *curNode = divisibleReverted->head;
    int subCount;
    while (curNode != NULL) {
        // TODO: remove debug output
        printf("--------------------buf: {");
        sList_Foreach(buf, printDigit);
        printf("}\n");
        printf("--------------------temp: {");
        sList_Foreach(temp, printDigit);
        printf("}\n");
        printf("--------------------result->digits: {");
        sList_Foreach(result->digits, printDigit);
        printf("}\n");

        while ((buf->head == NULL) || (longNumber_DigitsIsLess(buf, lnum2->digits))) {
            sList_Add(buf, curNode->val);
            curNode = curNode->next;
            // TODO: remove debug output
            printf("--------------------Added digit to buf.\n");
        }
        printf("--------------------Before subtracting:\n"); // TODO: remove
        printf("--------------------buf: {");
        sList_Foreach(buf, printDigit);
        printf("}\n");
        printf("--------------------lnum2->digits: {");
        sList_Foreach(lnum2->digits, printDigit);
        printf("}\n");

        subCount = 0;
        while (!longNumber_DigitsIsLess(buf, lnum2->digits)) {
            // QUE: how do I make this orthodoxial? There are too many memory operations
            printf("flag 1_0\n"); // TODO: remove
            sList_CopyTo(buf, &temp);

            printf("--------------------temp (after copying): {");
            sList_Foreach(temp, printDigit);
            printf("}\n");

            printf("flag 1_1\n"); // TODO: remove
            longNumber_DigitsSub(temp, lnum2->digits, buf);
            printf("flag 1_2\n"); // TODO: remove
            subCount++;

            printf("--------------------Subtracted.\n");
            printf("--------------------buf: {");
            sList_Foreach(buf, printDigit);
            printf("}\n");
        }

        printf("--------------------After subtracting:\n"); // TODO: remove
        printf("--------------------buf: {");
        sList_Foreach(buf, printDigit);
        printf("}\n");
        printf("--------------------lnum2->digits: {");
        sList_Foreach(lnum2->digits, printDigit);
        printf("}\n");

        sList_Add(result->digits, (void*)&subCount);
//        curNode = curNode->next;
    }

    // freeing memory
    sList_Dispose(temp);
    sList_Dispose(buf);
    sList_Dispose(divisibleReverted);
}

// changes the sign of {lnum} to opposite
void longNumber_DoNeg(LongNumber *lnum) {
	*lnum->sign = !(*lnum->sign);
}

// releases memory held by {lnum}
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

// writes the product of {lnum1} and {lnum2} to {result}
void longNumber_Mult(LongNumber *lnum1, LongNumber *lnum2, LongNumber *result) {
    longNumber_Clear(result);
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    longNumber_DigitsMultLongLong(lnum1->digits, lnum2->digits, &result->digits);
}

// prints {lnum} to stdio as decimal integer
void longNumber_Print(LongNumber *lnum) {
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
