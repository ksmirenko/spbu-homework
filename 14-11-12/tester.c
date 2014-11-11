/*
    Program for testing the singly linked list.
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "singleList.h"

int main() {
    char curCommand = 0;
    int arg;
    list_init();
    while (curCommand != 'q') {
        scanf("%c", &curCommand);
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
    }
    list_init();
    return 0;
}
