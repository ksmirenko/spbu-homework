#include <assert.h>
#include <stdio.h>
#include "slist.h"
#include "longnumber.h"

// reads a number until new line and writes it to a newly created singly linked list
SList* scanParseNumber() {
    SList *number = sList_Init(sizeof(int), NULL);
    char cur;
    scanf("%c", &cur);
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            fprintf(stderr, "Critical I/O error!\n");
        }
        assert((cur >= '0') && (cur <= '9'));
        int digit = (int)cur - '0';
        sList_Add(number, (void*)&digit);
        scanf("%c", &cur);
    }
    return number;
}

// reads a number until line break, writes it to a newly created LongNumber and returns the least
LongNumber* scanParseLongNumber() {
    LongNumber* number = longNumber_Init();
    char cur;
    scanf("%c", &cur);
    if (cur == '-') {
        *number->sign = 1;
        scanf("%c", &cur);
    }
    else {
        int temp = (int)cur - '0';
        sList_Add(number->digits, (void*)&temp);
        scanf("%c", &cur);
    }
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            fprintf(stderr, "Critical I/O error!\n");
        }
        assert((cur >= '0') && (cur <= '9'));
        int digit = (int)cur - '0';
        sList_Add(number->digits, (void*)&digit);
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

    int res = sList_IsLess(&num1, &num2);
    printf("(num1 < num2) == %d\n", res);

    IntNode *result = sList_Init();

    sList_Sum(&num1, &num2, &result);
    printf("num1 + num2 == ");
    sList_PrintRev(&result);

    sList_AbsDiff(&num1, &num2, &result);
    printf("num1 - num2 == ");
    sList_PrintRev(&result);

    sList_AbsDiff(&num2, &num1, &result);
    printf("num2 - num1 == ");
    sList_PrintRev(&result);

    // freeing memory
    sList_Dispose(&num1);
    sList_Dispose(&num2);
    sList_Dispose(&result);*/


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
            longNumber_Sum(num1, num2, result);
            break;
        case '-':
            longNumber_Sub(num1, num2, result);
            break;
        case '*':
            longNumber_Mult(num1, num2, result);
            break;
        default:
            printf("Error!\n");
            return -1;
    }

    longNumber_Print(num1);
    printf(" %c ", op);
    longNumber_Print(num2);
    printf(" == ");
    longNumber_Print(result);
    printf("\n");

    // freeing memory
    longNumber_Dispose(num1);
    longNumber_Dispose(num2);
    longNumber_Dispose(result);

    return 0;
}
