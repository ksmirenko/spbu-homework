#include <assert.h>
#include <stdio.h>
#include "slist.h"
#include "longnumber.h"

void printDigit1(void *elem) {
    printf("%d", *(int*)(elem));
}

// reads a number until new line and writes it to {number}
void scanParseNumber(SList *number) {
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

void debugProc1() {
    SList *orig = sList_Init(sizeof(int), NULL);

/*    int temp = 2;*/
/*    sList_Add(orig, (void*)&temp);*/

    printf("--------------------Print a number: ");
    scanParseNumber(orig);

    printf("--------------------orig: {");
    sList_Foreach(orig, printDigit1);
    printf("}\n");

    SList *copy = sList_Init(sizeof(int), NULL);

    printf("--------------------Copying...\n");
    sList_CopyTo(orig, &copy);
    printf("--------------------Copied.\n");

    printf("--------------------orig: {");
    sList_Foreach(orig, printDigit1);
    printf("}\n");
    printf("--------------------copy: {");
    sList_Foreach(copy, printDigit1);
    printf("}\n");

/*    sList_Clear(orig);*/
/*    sList_Clear(copy);*/
/*    printf("--------------------Cleared.\n");*/

/*    printf("--------------------orig: {");*/
/*    sList_Foreach(orig, printDigit1);*/
/*    printf("}\n");*/

    sList_Dispose(orig);
    sList_Dispose(copy);
    printf("--------------------Disposed.\n");
}

void debugProc2() {
    int temp;
    printf("--------------------Print a number: ");
    SList *orig = sList_Init(sizeof(int), NULL);
    scanParseNumber(orig);
    SList *result = sList_Init(sizeof(int), NULL);

    temp = 5;
    longNumber_DigitsMultLongShort(orig, temp, &result);
    

    printf("--------------------Calculated.\n");

    printf("--------------------Result: {");
    sList_Foreach(result, printDigit1);
    printf("}\n");

    sList_Dispose(orig);
    sList_Dispose(result);
}

int main() {
    debugProc2();
    return 0;
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
