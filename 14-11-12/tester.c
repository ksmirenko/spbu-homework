/*
    Program for testing the singly linked list.
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "singleList.h"

const int DEBUG = 1;

int main() {
    char curCommand = 0;
    int arg;
    list_init();
    while (curCommand != 'q') {
        scanf("%c", &curCommand);
        switch (curCommand) {
            case 'a':
                scanf("%d", &arg);
                if (DEBUG)
                    printf("ADD %d\n", arg);
                list_add(arg);
                break;
            case 'p':
                if (DEBUG)
                    printf("PRINT\n");
                list_print();
                break;
            case 'r':
                scanf("%d", &arg);
                if (DEBUG)
                    printf("REMOVE %d\n", arg);
                list_removeByValue(arg);
                break;
            default:
                break;
        }
    }
    if (DEBUG)
        printf("QUIT\n");
    list_init();
    return 0;
}
