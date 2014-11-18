#include <stdio.h>
#include <stdlib.h>
#include "intNode.h"
#include "longArithm.h"

const int ARITHM_BASE = 10;

// creates a new IntNode that is equal to reverted value of abs({num1} - {num2})
IntNode* intNode_AbsDiff(IntNode **num1, IntNode **num2) {
    // if num1 < num2, return (num2 - num1) instead
    if (intNode_IsLess(num1, num2)) {
        return intNode_AbsDiff(num2, num1);
    }

    IntNode *result = intNode_Init();
    IntNode *cur1 = *num1, *cur2 = *num2;
    int curDigit = 0, val1 = 0, val2 = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            val1 += cur1->val;
            cur1 = cur1->next;
        }
        if (cur2 != NULL) {
            val2 = cur2->val;
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
            curDigit = (ARITHM_BASE + val1 - val2) % ARITHM_BASE;
            val1 = -1;
        }
        if ((curDigit > 0) && (cur1 != NULL)) {
            intNode_Add(&result, curDigit % ARITHM_BASE);
        }        
        curDigit /= ARITHM_BASE;
    }
    return result;
}

// returns 1 if {num1} is less than {num2} and 0 otherwise
int intNode_IsLess(IntNode **num1, IntNode **num2) {
    IntNode *cur1 = *num1, *cur2 = *num2;
    int isFirstLess = 0, val1 = 0, val2 = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            val1 = cur1->val;
            cur1 = cur1->next;
        }
        else {
            return 1;
        }
        if (cur2 != NULL) {
            val2 = cur2->val;
            cur2 = cur2->next;
        }
        else {
            return 0;
        }
        if (val1 != val2) {
            isFirstLess = (val1 < val2);
        }
        // TODO: remove
//        printf("val1 == %d, val2 == %d, isFirstLess == %d\n", val1, val2, isFirstLess);
    }
    return isFirstLess;
}

// prints {number} to stdio as decimal integer
void longNumber_Print(LongNumber **number) {
    if ((*number)->sign) {
        printf("-");
    }
    IntNode *revDigits = intNode_Revert(&((*number)->digits));
    intNode_PrintRev(&revDigits);
    intNode_Dispose(&revDigits);
}

// creates a new IntNode that is equal to reverted value of ({num1} + {num2})
IntNode* intNode_Sum(IntNode **num1, IntNode **num2) {
    IntNode *result = intNode_Init();
    IntNode *cur1 = *num1, *cur2 = *num2;
    int curSum = 0;
    while ((cur1 != NULL) || (cur2 != NULL)) {
        if (cur1 != NULL) {
            curSum += cur1->val;
            cur1 = cur1->next;
        }
        if (cur2 != NULL) {
            curSum += cur2->val;
            cur2 = cur2->next;
        }
        intNode_Add(&result, curSum % ARITHM_BASE);
        curSum /= ARITHM_BASE;
    }
    return result;
}

// creates a new LongNumber that is equal to sum of {num1} and {num2}
LongNumber* longNumber_Add(LongNumber **num1, LongNumber **num2) {
    // TODO: implement
    return NULL;
}

// releases memory held by {number}
void longNumber_Dispose(LongNumber **number) {
    intNode_Dispose(&((*number)->digits));
    free(*number);
}

// creates an empty LongNumber
LongNumber* longNumber_Init() {
    LongNumber *temp = (LongNumber*)malloc(sizeof(LongNumber));
    if (temp == NULL) {
        printf("Error: out of memory!\n");
        return NULL;
    }
    temp->sign = 0;
    temp->digits = intNode_Init();
    return temp;
}

// creates a new LongNumber that is equal to difference between {num1} and {num2}
LongNumber* longNumber_Sub(LongNumber **num1, LongNumber **num2) {
    // TODO: implement
    return NULL;
}

// changes the sign of {num} to opposite
void longNumber_DoNeg(LongNumber **num) {
    // TODO: implement
}
