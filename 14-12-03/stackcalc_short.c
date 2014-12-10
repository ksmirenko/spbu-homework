/*
    Stack calculator (int numbers).
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "stack.h"

#define OPER_TYPE_SUM 0
#define OPER_TYPE_SUB 1
#define OPER_TYPE_MUL 2
#define OPER_TYPE_DIV 3

const int IS_INTERFACE_ENABLED = 1;
const int ARITHM_BASE = 10;
const char MAX_DIGIT_CHAR = '9';

Stack *stack;
int buf1, buf2, buf3;

int doOperation(int opType) {
    if (*stack->size < 2) {
        return -1;
    }
    stack_Pop(stack, (void*)&buf2);
    stack_Pop(stack, (void*)&buf1);
    switch (opType) {
        case OPER_TYPE_SUM:
            buf3 = buf1 + buf2;
            break;
        case OPER_TYPE_SUB:
            buf3 = buf1 - buf2;
            break;
        case OPER_TYPE_MUL:
            buf3 = buf1 * buf2;
            break;
        case OPER_TYPE_DIV:
            // checking division by zero
            if (!buf2) {
                return -2;
            }
            buf3 = buf1 / buf2;
            break;
        default:
            return -3;
    }
    stack_Push(stack, (void*)&buf3);
    return 0;
}

int main() {
    stack = stack_Init(sizeof(int), NULL, NULL);

    if (IS_INTERFACE_ENABLED) {
        printf("Welcome to Stack Calculator!\n");
        printf("The calculator accepts ints and operations: +, -, *, /.\n");
        printf("Please use Reverse Polish notation.\n");
        printf("Type '=' to print the calculated value and exit.\n");
    }

    int curNumber = 0, isReadingNumber = 0;
    char c;
    scanf("%c", &c);
    // main loop
    while (c != '=') {
        // digit
        if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) {
            curNumber = curNumber * ARITHM_BASE + (c - '0');
            isReadingNumber = 1;
        }

        // subtraction or beginning of a negative int
        else if (c == '-') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                stack_Dispose(stack);
           		return -1;
            }
            scanf("%c", &c);
            if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) { // negative int
                curNumber = 0 - (c - '0');
                isReadingNumber = 1;
            }
            else if ((c == ' ') || (c == '\n')) { // subtraction
                int errorCode = doOperation(OPER_TYPE_SUB);
                if (errorCode) {
                    fprintf(stderr,
                        "Error: invalid input commands (subtraction crashed); aborting...\n");
                    stack_Dispose(stack);
           		    return -1;
                }
            }
            else { // something erroneous
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' after '-')", c);
                stack_Dispose(stack);
           		return -1;
            }
        }

        // addition
        else if (c == '+') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                stack_Dispose(stack);
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_SUM);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (addition crashed); aborting...\n");
                stack_Dispose(stack);
           		return -1;
            }
        }

        // multiplication
        else if (c == '*') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                stack_Dispose(stack);
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_MUL);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (multiplication crashed); aborting...\n");
                stack_Dispose(stack);
           		return -1;
            }
        }
        
        // division
        else if (c == '/') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                stack_Dispose(stack);
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_DIV);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (division crashed); aborting...\n");
                stack_Dispose(stack);
           		return -1;
            }
        }

        // separator
        else if ((c == ' ') || (c == '\n') || (c == '\r')) {
            if (isReadingNumber) {
                isReadingNumber = 0;
                stack_Push(stack, (void*)&curNumber);
                curNumber = 0;
            }
        }

        // something erroneous
        else {
            fprintf(stderr, "Error: incorrect symbol '%c'; aborting...\n", c);
            stack_Dispose(stack);
            return -1;
        }

        scanf("%c", &c);
    }

    if (*stack->size == 1) {
        int answer;
        stack_Pop(stack, (void*)&answer);
        printf("%d\n", answer);
    }
    else {
        fprintf(stderr, "Error: invalid input commands.\n");
    }

    stack_Dispose(stack);
}
