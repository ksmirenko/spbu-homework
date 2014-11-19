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
    // testing operations with IntNode-s
    /*printf("Type the 1st number: ");
    IntNode *num1 = scanParseNumber();
    printf("Type the 2nd number: ");
    IntNode *num2 = scanParseNumber();

    int res = intNode_IsLess(&num1, &num2);
    printf("(num1 < num2) == %d\n", res);

    IntNode *result = intNode_Init();

    intNode_Sum(&num1, &num2, &result);
    printf("num1 + num2 == ");
    intNode_PrintRev(&result);

    intNode_AbsDiff(&num1, &num2, &result);
    printf("num1 - num2 == ");
    intNode_PrintRev(&result);

    intNode_AbsDiff(&num2, &num1, &result);
    printf("num2 - num1 == ");
    intNode_PrintRev(&result);

    // freeing memory
    intNode_Dispose(&num1);
    intNode_Dispose(&num2);
    intNode_Dispose(&result);*/


    // testing operations with LongNumber-s
    printf("Type the 1st number: ");
    LongNumber *num1 = scanParseLongNumber();
    printf("Type the 2nd number: ");
    LongNumber *num2 = scanParseLongNumber();
    printf("Type the operation (+/-): ");
    char op;    
    scanf("%c", &op);

    LongNumber *result = longNumber_Init();
    switch (op) {
        case '+':
            longNumber_Add(&num1, &num2, &result);
            break;
        case '-':
            longNumber_Sub(&num1, &num2, &result);
            break;
        default:
            printf("Error!\n");
            return -1;
    }

    longNumber_Print(&num1);
    printf(" %c ", op);
    longNumber_Print(&num2);
    printf(" == ");
    longNumber_Print(&result);
    printf("\n");

    // freeing memory
    longNumber_Dispose(&num1);
    longNumber_Dispose(&num2);
    longNumber_Dispose(&result);

    return 0;
}
