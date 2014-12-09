/*
    Stack calculator (int numbers).
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "stack.h"

const int IS_INTERFACE_ENABLED = 1;
const int ARITHM_BASE = 10;
const char MAX_DIGIT_CHAR = '9';

int doSum(Stack *stack) {
    if (*stack->size < 2) {
        return -1;
    }
    int numLast, numPreLast;
    stack_Pop(stack, (void*)&numLast);
    stack_Pop(stack, (void*)&numPreLast);
    int result = numPreLast + numLast;
    stack_Push(stack, (void*)&result);
    return 0;
}

int doNeg(Stack *stack) {
    if (*stack->size < 2) {
        return -1;
    }
    int numLast, numPreLast;
    stack_Pop(stack, (void*)&numLast);
    stack_Pop(stack, (void*)&numPreLast);
    int result = numPreLast - numLast;
    stack_Push(stack, (void*)&result);
    return 0;
}

int doMult(Stack *stack) {
    if (*stack->size < 2) {
        return -1;
    }
    int numLast, numPreLast;
    stack_Pop(stack, (void*)&numLast);
    stack_Pop(stack, (void*)&numPreLast);
    int result = numPreLast * numLast;
    stack_Push(stack, (void*)&result);
    return 0;
}

int doDiv(Stack *stack) {
    if (*stack->size < 2) {
        return -1;
    }
    int numLast, numPreLast;
    stack_Pop(stack, (void*)&numLast);
    if (numLast == 0) {
        return -2;
    }
    stack_Pop(stack, (void*)&numPreLast);
    int result = numPreLast / numLast;
    stack_Push(stack, (void*)&result);
    return 0;
}

int main() {
    Stack *stack = stack_Init(sizeof(int), NULL);

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
                int errorCode = doNeg(stack);
                if (!errorCode) {
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
            int errorCode = doSum(stack);
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
            int errorCode = doMult(stack);
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
            int errorCode = doDiv(stack);
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
