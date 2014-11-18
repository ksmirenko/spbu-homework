#include <stdio.h>
#include "intNode.h"

// reads a number until new line and writes it to a newly created singly linked list
IntNode* scanParseNumber() {
    IntNode *number = intNode_Init();
    intNode_Dispose(&number);
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

int main() {
    printf("Type the 1st number: ");
    IntNode *num1 = scanParseNumber();
    printf("Type the 2nd number: ");
    IntNode *num2 = scanParseNumber();

    int res = intNode_Less(&num1, &num2);
    printf("(num1 < num2) == %d\n", res);

    IntNode *result = (IntNode*)intNode_Sum(&num1, &num2); // FIXME: warning
    printf("num1 + num2 == ");
    intNode_PrintRev(&result);
    result = (IntNode*)intNode_AbsDiff(&num1, &num2); // FIXME: warning
    printf("num1 - num2 == ");
    intNode_PrintRev(&result);
    result = (IntNode*)intNode_AbsDiff(&num2, &num1); // FIXME: warning
    printf("num2 - num1 == ");
    intNode_PrintRev(&result);

    // freeing memory
    intNode_Dispose(&num1);
    intNode_Dispose(&num2);
    intNode_Dispose(&result);
    return 0;
}
