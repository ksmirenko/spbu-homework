#include <stdio.h>
#include "intNode.h"
#include "longArithm.h"

// reads a number until new line and writes it to a newly created singly linked list
IntNode* scanParseNumber() {
    IntNode *number = intNode_Init();
    char cur;
    scanf("%c", &cur);
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            return NULL;
        }
        int digit = (int)cur - '0';
        intNode_Add(&number, digit);
        scanf("%c", &cur);
    }
    return number;
}

// reads a number until new line and writes it to a newly created LongNumber
LongNumber* scanParseLongNumber() {
    LongNumber* number = longNumber_Init();
    char cur;
    scanf("%c", &cur);
    if (cur == '-') {
        number->sign = 1;
        scanf("%c", &cur);
    }
    else {
        intNode_Add(&(number->digits), (int)cur - '0');
        scanf("%c", &cur);
    }
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            return NULL;
        }
        int digit = (int)cur - '0';
        intNode_Add(&(number->digits), digit);
        scanf("%c", &cur);
    }
    return number;
}

int main() {
    /*printf("Type the 1st number: ");
    IntNode *num1 = scanParseNumber();
    printf("Type the 2nd number: ");
    IntNode *num2 = scanParseNumber();

    int res = intNode_IsLess(&num1, &num2);
    printf("(num1 < num2) == %d\n", res);

    IntNode *result = (IntNode*)intNode_Sum(&num1, &num2); // FIXME: warning
    printf("num1 + num2 == ");
    intNode_PrintRev(&result);
    result = (IntNode*)intNode_AbsDiff(&num1, &num2); // FIXME: warning
    printf("num1 - num2 == ");
    intNode_PrintRev(&result);
    result = (IntNode*)intNode_AbsDiff(&num2, &num1); // FIXME: warning
    printf("num2 - num1 == ");
    intNode_PrintRev(&result);

    // freeing memory
    intNode_Dispose(&num1);
    intNode_Dispose(&num2);
    intNode_Dispose(&result);*/

    printf("Type the 1st number: ");
    LongNumber *num1 = scanParseLongNumber();
    printf("Type the 2nd number: ");
    LongNumber *num2 = scanParseLongNumber();

    LongNumber *result = (LongNumber*)longNumber_Add(&num1, &num2);
    printf("num1 + num2 == ");
    longNumber_Print(&result);    

    // freeing memory
    longNumber_Dispose(&num1);

    return 0;
}
