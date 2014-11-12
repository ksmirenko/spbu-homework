/*
    Program for testing the singly linked list.
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "singleList.h"

const int DEBUG = 1;

int main(int argc, char **argv) {
    // opening files, if needed
    FILE *fileIn = NULL, *fileOut = NULL;
    if (argc > 1) {
        fileIn = freopen(argv[1], "r", stdin);
        if (fileIn == NULL) {
            printf("File IO error.\n");
            return -1;
        }
        if (DEBUG) {
            printf("Input file loaded.\n");
        }
    }
    if (argc > 2) {
        fileOut = freopen(argv[2], "w", stdout);
        if (DEBUG) {
            printf("Output file loaded.\n");
        }
    }

    char curCommand = 0;
    int arg;
    list_init();
    while (curCommand != 'q') {
        scanf("%c", &curCommand);
        if (DEBUG) {
            if (curCommand != ' ')
                printf("Command '%c'\n", curCommand);
        }
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
    if (DEBUG) {
        printf("Shutting down...\n");
    }
    // clearing resources
    list_init();
    if (DEBUG) {
        printf("List cleared.\n");
    }

    // closing files
    if (fileIn) {
        fclose(fileIn);
        if (DEBUG) {
            printf("Input file closed.\n");
        }
    }
    if (fileOut) {
        fclose(fileOut);
        if (DEBUG) {
            printf("Output file closed.\n");
        }
    }
    return 0;
}
