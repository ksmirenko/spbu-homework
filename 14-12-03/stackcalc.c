/*
    Stack calculator (long numbers).
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "stack.h"
#include "longnumber.h"

#define OPER_TYPE_SUM 0
#define OPER_TYPE_SUB 1
#define OPER_TYPE_MUL 2
#define OPER_TYPE_DIV 3

const int IS_INTERFACE_ENABLED = 0;
const int IS_PRINTING_STACK = 0;
const int ARITHM_BASE = 10;
const char MAX_DIGIT_CHAR = '9';

Stack *stack;
Lnum *curNumber;
Lnum *buf1, *buf2, *buf3;

void printStack() {
    printf("--------------[\n");
    if (stack->size > 0) {
        SlistNode *curNode = stack->list->head;
        while (curNode != NULL) {
            printf("--------------{");
            lnum_Print(*(Lnum**)curNode->val);
            printf("}\n");
            curNode = curNode->next;
        }
    }
    printf("--------------]\n");
}

int doOperation(int opType) {
    if (*stack->size < 2) {
        return -1;
    }
    lnum_Dispose(buf1);
    lnum_Dispose(buf2);
    lnum_Clear(buf3);

    stack_Pop(stack, (void*)&buf2);
    stack_Pop(stack, (void*)&buf1);

    switch (opType) {
        case OPER_TYPE_SUM:
            lnum_Sum(buf1, buf2, buf3);
            break;
        case OPER_TYPE_SUB:
            lnum_Sub(buf1, buf2, buf3);
            break;
        case OPER_TYPE_MUL:
            lnum_Mult(buf1, buf2, buf3);
            break;
        case OPER_TYPE_DIV:
            // checking division by zero
            if ((buf2->digits != NULL)
            && (*(int*)buf2->digits->head->val == 0)
            && (buf2->digits->head->next == NULL)) {
                return -2;
            }
            lnum_Div(buf1, buf2, buf3);
            break;
        default:
            return -3;
    }
    stack_Push(stack, (void*)&buf3);
    if (IS_PRINTING_STACK) {
        printStack();
    }
    return 0;
}

void memFree() {
    lnum_Dispose(buf1);
    lnum_Dispose(buf2);
    lnum_Dispose(buf3);
    lnum_Dispose(curNumber);
    stack_Dispose(stack);
}

int main() {
    stack = stack_Init(sizeof(Lnum*), lnum_CloneDelegate, lnum_DisposeDelegate);
    curNumber = lnum_Init();
    buf1 = lnum_Init();
    buf2 = lnum_Init();
    buf3 = lnum_Init();

    if (IS_INTERFACE_ENABLED) {
        printf("Welcome to the Great Stack Calculator!\n");
        printf("The calculator accepts long numbers and operations: +, -, *, /.\n");
        printf("Please use Reverse Polish notation.\n");
        printf("Type '=' to print the calculated value and exit.\n");
    }

    int isReadingNumber = 0;
    char c;
    scanf("%c", &c);
    // main loop
    while (c != '=') {
        // digit
        if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) {
            lnum_DigitAdd(curNumber, c - '0');
            isReadingNumber = 1;
        }

        // subtraction or beginning of a negative int
        else if (c == '-') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                memFree();
           		return -1;
            }
            scanf("%c", &c);
            if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) { // negative int
                lnum_Clear(curNumber);
                lnum_DoNeg(curNumber);
                lnum_DigitAdd(curNumber, c - '0');
                isReadingNumber = 1;
            }
            else if ((c == ' ') || (c == '\n')) { // subtraction
                int errorCode = doOperation(OPER_TYPE_SUB);
                if (errorCode) {
                    fprintf(stderr,
                        "Error: invalid input commands (subtraction crashed: %d); aborting...\n",
                        errorCode);
                    memFree();
           		    return -1;
                }
            }
            else { // something erroneous
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' after '-')", c);
                memFree();
           		return -1;
            }
        }

        // addition
        else if (c == '+') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                memFree();
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_SUM);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (addition crashed); aborting...\n");
                memFree();
           		return -1;
            }
        }

        // multiplication
        else if (c == '*') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                memFree();
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_MUL);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (multiplication crashed); aborting...\n");
                memFree();
           		return -1;
            }
        }
        
        // division
        else if (c == '/') {
            if (isReadingNumber) {
                fprintf(stderr, "Error: incorrect input format; aborting...\n");
                fprintf(stderr, "(\tgot symbol '%c' while reading a number)", c);
                memFree();
           		return -1;
            }
            int errorCode = doOperation(OPER_TYPE_DIV);
            if (errorCode) {
                fprintf(stderr, "Error: invalid input commands (division crashed); aborting...\n");
                memFree();
           		return -1;
            }
        }

        // separator
        else if ((c == ' ') || (c == '\n') || (c == '\r')) {
            if (isReadingNumber) {
                isReadingNumber = 0;                
                stack_Push(stack, (void*)&curNumber);
                lnum_Clear(curNumber);
                if (IS_PRINTING_STACK) {
                    printStack();
                }
            }
        }

        // got something erroneous
        else {
            fprintf(stderr, "Error: incorrect symbol '%c'; aborting...\n", c);
            memFree();
            return -1;
        }

        scanf("%c", &c);
    }
    
    if (*stack->size == 1) {
        // printing the answer
        stack_Pop(stack, (void*)&curNumber);
        lnum_Print(curNumber);
        printf("\n");
    }
    else {
        fprintf(stderr, "Error: invalid input commands.\n");
    }

    memFree();
    return 0;
}
