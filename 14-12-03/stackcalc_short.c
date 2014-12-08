#include <stdio.h>
#include "stack.h"

const int IS_INTERFACE_ENABLED = 1;
const int ARITHM_BASE = 10;
const char MAX_DIGIT_CHAR = '9';

int doSum(Stack *stack) {
    // TODO
    return 0;
}

int doNeg(Stack *stack) {
    // TODO
    return 0;
}

int doMult(Stack *stack) {
    // TODO
    return 0;
}

int doDiv(Stack *stack) {
    // TODO
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

    int curNumber = 0, numLast, numPreLast, isReadingNumber = 0;
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
                break;
            }
            scanf("%c", &c);
            if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) { // negative int
                curNumber = 0 - (c - '0');
                isReadingNumber = 1;
            }
            else if ((c == ' ') || (c == '\n')) { // subtraction
                int result = doNeg(stack);
                if (!result) {
                    fprintf(stderr, "Error: invalid input commands; aborting...\n");
                    break;
                }
            }
            else { // something erroneous
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                break;
            }
        }

        // addition
        else if (c == '+') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                break;
            }
            int result = doSum(stack);
            if (!result) {
                fprintf(stderr, "Error: invalid input commands; aborting...\n");
                break;
            }
        }

        // multiplication
        else if (c == '*') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                break;
            }
            int result = doMult(stack);
            if (!result) {
                fprintf(stderr, "Error: invalid input commands; aborting...\n");
                break;
            }
        }
        
        // division
        else if (c == '/') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                break;
            }
            int result = doDiv(stack);
            if (!result) {
                fprintf(stderr, "Error: invalid input commands; aborting...\n");
                break;
            }
        }

        // separator
        else if ((c == ' ') || (c == '\n')) {
            if (isReadingNumber) {
                isReadingNumber = 0;
                stack_Push(stack, (void*)&curNumber);
                curNumber = 0;
            }
        }

        // something erroneous
        else {
            fprintf(stderr, "Error: incorrect input format; aborting...\n");
            break;
        }

        scanf("%c", &c);
    }

    // TODO: output

    stack_Dispose(stack);
}
