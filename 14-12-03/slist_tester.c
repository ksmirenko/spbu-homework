/*
    Program for testing the (generic) singly linked list.
	Call format: <launcher> [<input file> [<output file>]]
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>
#include "slist.h"

const int DEBUG = 0;

void printInt(void *elem) {
    printf("%d ", *(int*)(elem));
}

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
	
	// creating SList<int>
	SList *list = sList_Init(sizeof(int), NULL, NULL);

    char curCommand = 0;
    int arg;
    while (curCommand != 'q') {
        scanf("%c", &curCommand);
        if (DEBUG) {
            if (curCommand != ' ')
                printf("Command '%c'\n", curCommand);
        }
        switch (curCommand) {
            case 'a':
                scanf("%d", &arg);
                sList_Add(list, (void*)(&arg));
                break;
            case 'p':
                sList_Foreach(list, printInt);
                printf("\n");
                break;
            case 'r':
                scanf("%d", &arg);
                sList_RemoveFirstOcc(list, (void*)(&arg));
                break;
            default:
                break;
        }
    }
    if (DEBUG) {
        printf("Shutting down...\n");
    }
	
    // freeing memory
    sList_Dispose(list);
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
