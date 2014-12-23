/*
    Stack calculator (long numbers).
    Call format: <launcher> [<input file> [<output file>]]
    Author: Kirill Smirenko, group 171
*/

#include <assert.h>
#include <stdio.h>
#include "stack.h"
#include "longnumber.h"

#define OPER_TYPE_SUM 0
#define OPER_TYPE_SUB 1
#define OPER_TYPE_MUL 2
#define OPER_TYPE_DIV 3

#define ERR_DIV_BY_ZERO     -1
#define ERR_NOT_ENOUGH_ARGS -2
#define ERR_UNKNOWN_COMMAND -3
#define ERR_LOL             -4

const int IS_INTERFACE_ENABLED = 0;
const int IS_PRINTING_STACK = 0;
const int ARITHM_BASE = 10;
const char MAX_DIGIT_CHAR = '9';

Stack *stack;
Lnum *curNumber;
Lnum *buf1, *buf2, *buf3;
FILE *fileIn, *fileOut;

void printStack() {
    fprintf(stdout, "[");
    if (stack->size > 0) {
        SlistNode *curNode = stack->list->head;
        while (curNode != NULL) {
            lnum_Print(*(Lnum**)curNode->val);
            curNode = curNode->next;
            if (curNode != NULL) {
                fprintf(stdout, "; ");
            }
        }
    }
    fprintf(stdout, "]\n");
}

void printStackDebug() {
    fprintf(stderr, "--------------STACK:\n");
    fprintf(stderr, "--------------[\n");
    if (stack->size > 0) {
        SlistNode *curNode = stack->list->head;
        while (curNode != NULL) {
            fprintf(stderr, "--------------");
            lnum_PrintDebug(*(Lnum**)curNode->val);
            curNode = curNode->next;
        }
    }
    fprintf(stderr, "--------------]\n");
    fprintf(stderr, "--------------\n");
}

int doOperation(int opType) {
    if (*stack->size < 2) {
        return ERR_NOT_ENOUGH_ARGS;
    }
    lnum_Dispose(buf1);
    lnum_Dispose(buf2);
    lnum_Clear(buf3);

    stack_Pop(stack, (void*)&buf1);
    stack_Pop(stack, (void*)&buf2);

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
                return ERR_DIV_BY_ZERO;
            }
            lnum_Div(buf1, buf2, buf3);
            break;
        default:
            return ERR_LOL;
    }
    stack_Push(stack, (void*)&buf3);
    if (IS_PRINTING_STACK) {
        printStackDebug();
    }
    return 0;
}

void memFree() {
    lnum_Dispose(buf1);
    lnum_Dispose(buf2);
    lnum_Dispose(buf3);
    lnum_Dispose(curNumber);
    stack_Dispose(stack);
    // closing files
    if (fileIn != NULL) {
        fclose(fileIn);
    }
    if (fileOut != NULL) {
        fclose(fileOut);
    }
}

int main(int argc, char **argv) {
    fileIn = NULL, fileOut = NULL;
    // opening files, if needed
    if (argc > 1) {
        fileIn = freopen(argv[1], "r", stdin);
        if (fileIn == NULL) {
            fprintf(stderr, "File IO error.\n");
            return ERR_LOL;
        }
    }
    if (argc > 2) {
        fileOut = freopen(argv[2], "w", stdout);
    }
    
    // allocating memory
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
    char c = getchar();
    // main loop
    while (c != EOF) {
        if (c == '=') {
            if (*stack->size > 0) {
                lnum_Print(*(Lnum**)stack->list->head->val);
            }
            else {
                fprintf(stdout, "Not enough arguments\n");
                memFree();
                return 1;
            }
        }
        
        // digit
        if ((c >= '0') && (c <= MAX_DIGIT_CHAR)) {
            lnum_DigitAdd(curNumber, c - '0');
            isReadingNumber = 1;
        }

        // subtraction or beginning of a negative int
        else if (c == '-') {
            if (isReadingNumber) {
                fprintf(stdout, "Unknown command\n");
                memFree();
           		return 1;
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
                    switch (errorCode) {
                        case ERR_NOT_ENOUGH_ARGS:
                            fprintf(stdout, "Not enough arguments\n");
                            break;
                        default:
                            fprintf(stdout, "Unknown error, lol\n");
                    }
                    memFree();
               		return 1;
                }
            }
            else { // something erroneous
                fprintf(stdout, "Unknown command\n");
                memFree();
           		return 1;
            }
        }

        // addition
        else if (c == '+') {
            if (isReadingNumber) {
                fprintf(stdout, "Unknown command\n");
                memFree();
           		return 1;
            }
            int errorCode = doOperation(OPER_TYPE_SUM);
            if (errorCode) {
                switch (errorCode) {
                    case ERR_NOT_ENOUGH_ARGS:
                        fprintf(stdout, "Not enough arguments\n");
                        break;
                    default:
                        fprintf(stdout, "Unknown error, lol\n");
                }
                memFree();
           		return 1;
            }
        }

        // multiplication
        else if (c == '*') {
            if (isReadingNumber) {
                fprintf(stdout, "Unknown command\n");
                memFree();
           		return 1;
            }
            int errorCode = doOperation(OPER_TYPE_MUL);
            if (errorCode) {
                switch (errorCode) {
                    case ERR_NOT_ENOUGH_ARGS:
                        fprintf(stdout, "Not enough arguments\n");
                        break;
                    default:
                        fprintf(stdout, "Unknown error, lol\n");
                }
                memFree();
           		return 1;
            }
        }
        
        // division
        else if (c == '/') {
            if (isReadingNumber) {
                fprintf(stdout, "Unknown command\n");
                memFree();
           		return 1;
            }
            int errorCode = doOperation(OPER_TYPE_DIV);       
            if (errorCode) {
                switch (errorCode) {
                    case ERR_NOT_ENOUGH_ARGS:
                        fprintf(stdout, "Not enough arguments\n");
                        break;
                    case ERR_DIV_BY_ZERO:
                        fprintf(stdout, "Division by zero\n");
                        break;
                    default:
                        fprintf(stdout, "Unknown error, lol\n");
                }
                memFree();
           		return 1;
            }
        }

        // separator
        else if ((c == ' ') || (c == '\n') || (c == '\r')) {
            if (isReadingNumber) {
                isReadingNumber = 0;                
                stack_Push(stack, (void*)&curNumber);
                lnum_Clear(curNumber);
                if (IS_PRINTING_STACK) {
                    printStackDebug();
                }
            }
        }

        // got something erroneous
        else {
            fprintf(stdout, "Unknown command\n");
            memFree();
            return 1;
        }
        
        c = getchar();
    }
    
    // after-loop output
    printStack();

    memFree();
    return 0;
}
