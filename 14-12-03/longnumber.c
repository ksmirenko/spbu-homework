#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "slist.h"
#include "longnumber.h"

const int LONG_NUMBER_BASE = 10; // base of the used numeric system

// a private delegate that is passed to slist_Foreach; prints one digit to stdout
void printDigit(void *elem) {
    printf("%d", *(int*)(elem));
}

// a private delegate that is passed to slist_Foreach; prints one digit to stderr
void printDigitDebug(void *elem) {
    fprintf(stderr, "%d", *(int*)(elem));
}

// debug function that prints SList as a number
void printDigitsDebug(Slist *digits) {
    assert(digits != NULL);
    fprintf(stderr, "{");
    Slist *revDigits = slist_Init(digits->nodeSize, digits->copyFunc,
        digits->freeFunc);
    slist_RevertTo(digits, revDigits);
    slist_Foreach(revDigits, printDigitDebug);
    slist_Dispose(revDigits);
    fprintf(stderr, "}\n");
}

// a delegate that is passed to Slist as a CopyFunction
void lnum_CloneDelegate(void *memTo, void *memFrom) {
    // initializing Lnum at memTo
    *(Lnum**)memTo = lnum_Init();

    Lnum *lnumFrom = *(Lnum**)memFrom;
    Lnum *lnumTo = *(Lnum**)memTo;
    
    assert(lnumTo != NULL);
    *lnumTo->sign = *lnumFrom->sign;
    slist_CopyTo(lnumFrom->digits, &lnumTo->digits);
}

// writes a deep copy of {lnumFrom} to {lnumTo}
void lnum_CopyTo(Lnum *lnumFrom, Lnum *lnumTo) {
    assert(lnumFrom != NULL);
    assert(lnumTo != NULL);
    *lnumTo->sign = *lnumFrom->sign;
    slist_CopyTo(lnumFrom->digits, &lnumTo->digits);
}

// clears {lnum} to initial state (*digits == empty Slist, *sign == 0)
void lnum_Clear(Lnum *lnum) {
	assert(lnum != NULL);
    slist_Clear(lnum->digits);
    *lnum->sign = 0;
}

// adds {digit} to {lnum} as a new least significant digit
void lnum_DigitAdd(Lnum *lnum, int digit) {
    assert(lnum != NULL);
    int buf = digit;
    slist_Add(lnum->digits, (void*)&buf);
}

// deletes leading (most significant) zeroes in {digits}
void lnum_DigitsDeleteLeadingZeroes(Slist *digits) {
    assert(digits != NULL);
    SlistNode *curNode = digits->head->next;
    int lastNonZeroIndex = 0, i = 1;
    while (curNode != NULL) {
        if (*(int*)curNode->val) {
            lastNonZeroIndex = i;
        }
        ++i;
        curNode = curNode->next;
    }
    curNode = digits->head->next;
    SlistNode *prevNode = digits->head;
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
int lnum_DigitsIsLess(Slist *digits1, Slist *digits2) {
	SlistNode *cur1 = digits1->head, *cur2 = digits2->head;
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
void lnum_DigitsMultLongLong(Slist *digits1, Slist *digits2, Slist **result) {
    assert(digits1 != NULL);
    assert(digits2 != NULL);

    slist_Clear(*result);
    int zero = 0;
    slist_Add(*result, (void*)&zero);
    Slist *localResult = slist_Init(digits1->nodeSize, digits1->copyFunc, digits1->freeFunc);
    Slist *temp = slist_Init(digits1->nodeSize, digits1->copyFunc, digits1->freeFunc);

    SlistNode *curNode2 = digits2->head;
    int shift = 0, i;
    while (curNode2 != NULL) {
        lnum_DigitsMultLongShort(digits1, *(int*)curNode2->val, &localResult);
        for (i = 0; i < shift; ++i) {
            slist_Add(localResult, (void*)&zero);
        }
        slist_CopyTo(*result, &temp); 
        lnum_DigitsSum(temp, localResult, *result);
        slist_Revert(result);

        ++shift;
        curNode2 = curNode2->next;
    }

    slist_Dispose(localResult);
    slist_Dispose(temp);
}

// writes the (not reverted) value of ({digits} * {num}) to {result}
void lnum_DigitsMultLongShort(Slist *digits, int num, Slist **result) {
    slist_Clear(*result);
    int carry = 0, buf;
    SlistNode *curDigit = digits->head;
    while (curDigit != NULL) {
        carry += *(int*)curDigit->val * num;
        buf = carry % LONG_NUMBER_BASE;
        slist_Add(*result, (void*)&buf);
        carry /= LONG_NUMBER_BASE;
        curDigit = curDigit->next;
    }
    if (carry) {
        slist_Add(*result, (void*)&carry);
    }
    slist_Revert(result);
}

// writes the reverted value of abs({digits1} - {digits2}) to {result}
void lnum_DigitsSub(Slist *digits1, Slist *digits2, Slist *result) {
	// if num1 < num2, return (num2 - num1) instead
    if (lnum_DigitsIsLess(digits1, digits2)) {
        lnum_DigitsSub(digits2, digits1, result);
        return;
    }

    slist_Clear(result);
    SlistNode *cur1 = digits1->head, *cur2 = digits2->head;
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
            slist_Add(result, (void*)&temp);
        }        
        curDigit /= LONG_NUMBER_BASE;
    }
    // if result is NULL, then it equals to zero; writing 0
    if (result->head == NULL) {
        int temp = 0;
        slist_Add(result, (void*)&temp);
    }
    // deleting leading zeroes (they are in the head for {result} is reverted)
    SlistNode *curHead = result->head;
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
void lnum_DigitsSum(Slist *digits1, Slist *digits2, Slist *result) {
	slist_Clear(result);
    SlistNode *cur1 = digits1->head, *cur2 = digits2->head;
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
        slist_Add(result, (void*)&temp);
        curSum /= LONG_NUMBER_BASE;
    }
    if (curSum) {
        temp = curSum % LONG_NUMBER_BASE;
        slist_Add(result, (void*)&temp);
    }
}

// writes the result of integer division {lnum1} / {lnum2} to {result}
void lnum_Div(Lnum *lnum1, Lnum *lnum2, Lnum *result) {
    assert(lnum1 != NULL);
    assert(lnum2 != NULL);
    slist_Clear(result->digits);
    // checking division by zero
    if ((lnum2->digits != NULL)
    && (*(int*)lnum2->digits->head->val == 0)
    && (lnum2->digits->head->next == NULL)) {
        *result->sign = 0;
        int zero = 0;
        slist_Add(result->digits, (void*)&zero);
        return;
    }

    if (lnum_DigitsIsLess(lnum1->digits, lnum2->digits)) {
        if (*lnum1->sign) {
            *result->sign = *lnum1->sign ^ *lnum2->sign;
            int one = 1;
            slist_Add(result->digits, (void*)&one);
            return;
        }
        else {
            *result->sign = 0;
            int zero = 0;
            slist_Add(result->digits, (void*)&zero);
            return;
        }
    }
    
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    
    // initializing temp vars
    Slist *divisibleReverted = slist_Init(lnum1->digits->nodeSize,
        lnum1->digits->copyFunc, lnum1->digits->freeFunc);
    Slist *buf = slist_Init(divisibleReverted->nodeSize, divisibleReverted->copyFunc,
        divisibleReverted->freeFunc);
    Slist *temp = slist_Init(divisibleReverted->nodeSize, divisibleReverted->copyFunc,
        divisibleReverted->freeFunc);

    slist_RevertTo(lnum1->digits, divisibleReverted);
    SlistNode *curNode = divisibleReverted->head;
    int subCount;
    int passedLeadingZeroes = 0;
    
    // the process of division
    while (curNode != NULL) {
        // adding next digit
        slist_Add(buf, curNode->val);
        curNode = curNode->next;

        subCount = 0;
        if (lnum_DigitsIsLess(buf, lnum2->digits)) {
            // can't subtract
            if (passedLeadingZeroes) {
                slist_Add(result->digits, (void*)&subCount);              
            }
        }
        else {
            // can subtract
            // subtracting while we can
            while (!lnum_DigitsIsLess(buf, lnum2->digits)) {
                slist_CopyTo(buf, &temp);
                lnum_DigitsSub(temp, lnum2->digits, buf);
                slist_Revert(&buf);
                subCount++;
            }
            // writing subCount to result
            slist_Add(result->digits, (void*)&subCount);
            // if buf == {0}, making buf {}
            if ((*(int*)buf->head->val == 0)
            && (buf->head->next == NULL)) {
                int trash;
                slist_Remove(buf, (void*)&trash);
            }
            // saying that there will be no more leading zeroes in result
            if (!passedLeadingZeroes) {
                passedLeadingZeroes = 1;
            }
        }
    }
    
    // if lnum1 < 0, incrementing result modulo by 1 (to follow algebra rules)
    if (*lnum1->sign) {
        Lnum *one = lnum_Init();
        *one->sign = !*lnum2->sign;
        lnum_DigitAdd(one, 1);
        
        Lnum *tempL = lnum_Init();
        lnum_CopyTo(result, tempL);
        lnum_Sum(tempL, one, result);
        
        lnum_Dispose(tempL);
        lnum_Dispose(one);
    }

    // freeing memory
    slist_Dispose(temp);
    slist_Dispose(buf);
    slist_Dispose(divisibleReverted);
}

// releases memory held by {lnum}
void lnum_Dispose(Lnum *lnum) {
    if (lnum == NULL) {
        return;
    }
    slist_Dispose(lnum->digits);
    free(lnum->sign);
    free(lnum);
}

// a delegate that is passed to Slist as a FreeFunction
void lnum_DisposeDelegate(void *lnum) {
    assert(lnum != NULL);
    lnum_Dispose(*(Lnum**)lnum);
}

// changes the sign of {lnum} to opposite
void lnum_DoNeg(Lnum *lnum) {
	*lnum->sign = !(*lnum->sign);
}

// creates and returns an empty Lnum
Lnum* lnum_Init() {
	Lnum *temp = (Lnum*)malloc(sizeof(Lnum));
    if (temp == NULL) {
        fprintf(stderr, "Error: not enough memory to create a Lnum!\n");
        return NULL;
    }
	assert(temp != NULL);
    temp->digits = slist_Init(sizeof(int), NULL, NULL);
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
void lnum_Mult(Lnum *lnum1, Lnum *lnum2, Lnum *result) {
    lnum_Clear(result);
    *result->sign = *lnum1->sign ^ *lnum2->sign;
    lnum_DigitsMultLongLong(lnum1->digits, lnum2->digits, &result->digits);
}

// prints {lnum} to stdout as decimal integer
void lnum_Print(Lnum *lnum) {
    if (lnum == NULL) {
        printf("NULL");
        return;
    }
    assert(lnum->sign != NULL);
    assert(lnum->digits != NULL);
    if (*lnum->sign) {
        printf("-");
    }
    Slist *revDigits = slist_Init(lnum->digits->nodeSize, lnum->digits->copyFunc,
        lnum->digits->freeFunc);
    slist_RevertTo(lnum->digits, revDigits);
    slist_Foreach(revDigits, printDigit);
    slist_Dispose(revDigits);
}

// prints {lnum} to stderr as decimal integer
void lnum_PrintDebug(Lnum *lnum) {
    if (lnum == NULL) {
        fprintf(stderr, "NULL");
        return;
    }
    assert(lnum->sign != NULL);
    assert(lnum->digits != NULL);
    fprintf(stderr, "{");
    if (*lnum->sign) {
        fprintf(stderr, "-");
    }
    Slist *revDigits = slist_Init(lnum->digits->nodeSize, lnum->digits->copyFunc,
        lnum->digits->freeFunc);
    slist_RevertTo(lnum->digits, revDigits);
    slist_Foreach(revDigits, printDigitDebug);
    slist_Dispose(revDigits);
    fprintf(stderr, "}\n");
}

// writes the difference between {lnum1} and {lnum2} to {result}
void lnum_Sub(Lnum *lnum1, Lnum *lnum2, Lnum *result) {
	lnum_DoNeg(lnum2);
    lnum_Sum(lnum1, lnum2, result);
    lnum_DoNeg(lnum2);
}

// writes the sum of {lnum1} and {lnum2} to {result}
void lnum_Sum(Lnum *lnum1, Lnum *lnum2, Lnum *result) {
	lnum_Clear(result);
    if (*lnum1->sign == *lnum2->sign) {
        *result->sign = *lnum1->sign;
        lnum_DigitsSum(lnum1->digits, lnum2->digits, result->digits);
    }
    else {
        if (lnum_DigitsIsLess(lnum1->digits, lnum2->digits)) {
            *result->sign = *lnum2->sign;
        }
        else {
            *result->sign = *lnum1->sign;
        }
        lnum_DigitsSub(lnum1->digits, lnum2->digits, result->digits);
    }
    slist_Revert(&result->digits);
}
