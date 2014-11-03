/*
    Task: exploiting buffer overflow, make a code injection and make the program call a completely non-used function.
    Author: Kirill Smirenko, group 171
*/

// to exploit, type: 000000000000000000000000c@

#include <stdio.h>

void f1() {
    char buf[4];
    
    printf("Stack:\n");
    printf("\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n");
    
    printf("Type some data (promise it won't overrun the 4-byte buffer):\n");
    scanf("%s", buf);
    printf("You typed: %s\n", buf);
    
    printf("Stack:\n");
    printf("\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n\t%p\n");
    return;
}

void f2() {
    printf("Sorry, my account was hacked!\n");
}

int main() {
    printf("f1:\t%p\nf2:\t%p\nMain:\t%p\n", &f1, &f2, &main);
    f1();
    printf("Everything seems to be in order...\n");
    return 0;
}
