#include <assert.h>
#include <stdio.h>
#include "slist.h"
#include "longnumber.h"

Lnum *lnum1, *lnum2, *result;

void printDigit1(void *elem) {
    printf("%d", *(int*)(elem));
}

// reads a number until line break, writes it to a newly created Lnum and returns the least
Lnum* scanParseLongNumber() {
    Lnum* number = lnum_Init();
    char cur;
    scanf("%c", &cur);
    if (cur == '-') {
        *number->sign = 1;
        scanf("%c", &cur);
    }
    else {
        lnum_DigitAdd(number, (int)cur - '0');
        scanf("%c", &cur);
    }
    while (cur != '\n') {
        if ((cur < '0') || (cur > '9')) {
            fprintf(stderr, "Critical I/O error!\n");
            lnum_Dispose(number);
        }
        assert((cur >= '0') && (cur <= '9'));
        lnum_DigitAdd(number, (int)cur - '0');
        scanf("%c", &cur);
    }
    return number;
}

void memFree() {
    lnum_Dispose(lnum1);
    lnum_Dispose(lnum2);
    lnum_Dispose(result);
}

int main() {
    printf("Type the 1st number: ");
    lnum1 = scanParseLongNumber();
    printf("Type the 2nd number: ");
    lnum2 = scanParseLongNumber();
    printf("Type the operation (+,-,*,/): ");
    char op;    
    scanf("%c", &op);

    result = lnum_Init();
    switch (op) {
        case '+':
            lnum_Sum(lnum1, lnum2, result);
            break;
        case '-':
            lnum_Sub(lnum1, lnum2, result);
            break;
        case '*':
            lnum_Mult(lnum1, lnum2, result);
            break;
        case '/':
            lnum_Div(lnum1, lnum2, result);
            break;
        default:
            printf("I/O error\n");
            memFree();
            return -1;
    }

    lnum_Print(lnum1);
    printf(" %c ", op);
    lnum_Print(lnum2);
    printf(" == ");
    lnum_Print(result);
    printf("\n");

    memFree();

    return 0;
}
