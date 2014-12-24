/* 
    A program that generates commands for the SingleList tester.
    Format: ./a.out <file name> <"a" commands number>
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        return -1;
    }
    FILE *fileOut = freopen(argv[1], "w", stdout);
    int n = atoi(argv[2]);
    srand(time(NULL));
    int i;
    for (i = 0; i < n; ++i) {
        printf("a %d ", rand());
    }   
    printf("p q\n");
    fclose(fileOut);
    return 0;
}
