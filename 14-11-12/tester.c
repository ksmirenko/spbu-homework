/*
    Program for testing the singly linked list.
    This file was created to 
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "singleList.h"

int main() {
    int curCommand = 255, arg;
    int hack = 0;
    list_init();
    while ((curCommand != 'q') && (hack < 5)) {
        scanf("%c", &curCommand);
        printf("Command: '%c'\n", curCommand);
        switch (curCommand) {
            case 'a':
                scanf("%d", &arg);
                list_add(arg);
                break;
            case 'p':
                list_print();
                break;
            case 'r':
                scanf("%d", &arg);
                list_removeByValue(arg);
                break;
            default:
                break;
        }
        hack++;
    }
    list_init();
    return 0;
}
