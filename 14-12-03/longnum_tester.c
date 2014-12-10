#include <assert.h>
#include <stdio.h>
#include "slist.h"
#include "longnumber.h"

LongNumber *lnum1, *lnum2, *result;

void printDigit1(void *elem) {
    printf("%d", *(int*)(elem));
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
        longNumber_DigitAdd(number, (int)cur - '0');
        scanf("%c", &cur);
    }
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            fprintf(stderr, "Critical I/O error!\n");
            longNumber_Dispose(number);
        }
        assert((cur >= '0') && (cur <= '9'));
        longNumber_DigitAdd(number, (int)cur - '0');
        scanf("%c", &cur);
    }
    return number;
}

void memFree() {
    longNumber_Dispose(lnum1);
    longNumber_Dispose(lnum2);
    longNumber_Dispose(result);
}

int main() {
    printf("Type the 1st number: ");
    lnum1 = scanParseLongNumber();
    printf("Type the 2nd number: ");
    lnum2 = scanParseLongNumber();
    printf("Type the operation (+,-,*,/): ");
    char op;    
    scanf("%c", &op);

    result = longNumber_Init();
    switch (op) {
        case '+':
            longNumber_Sum(lnum1, lnum2, result);
            break;
        case '-':
            longNumber_Sub(lnum1, lnum2, result);
            break;
        case '*':
            longNumber_Mult(lnum1, lnum2, result);
            break;
        case '/':
            longNumber_Div(lnum1, lnum2, result);
            break;
        default:
            printf("I/O error\n");
            memFree();
            return -1;
    }

    longNumber_Print(lnum1);
    printf(" %c ", op);
    longNumber_Print(lnum2);
    printf(" == ");
    longNumber_Print(result);
    printf("\n");

    memFree();

    return 0;
}
