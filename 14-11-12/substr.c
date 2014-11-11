/*
    Task: Search the first index of a substring in a string
        (naive algorythm, good code)
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>

const int MAX_LENGTH = 50;

int firstOcc(char *s, char *subs) {
    int i = 0, j = 0;
    while (s[i] != '\0') {
        while ((s[i] != '\0') && (s[i] != subs[0])) {
             ++i;
        }
        if (s[i] == '\0') {
            return -1;
        }
        
        j = 0;
        while ((s[i + j] != '\0') && (subs[j] != '\0') &&
            (s[i + j] == subs[j])) {
            ++j;
        }
        
        if (subs[j] == '\0') {
            return i;
        }
        if (s[i] == '\0') {
            return -1;
        }
    }
}

int main() {
    char *str, *substr;
    
    printf("Type a string (no more than %d characters: ", MAX_LENGTH);
    fgets(str, MAX_LENGTH, stdin);
    printf("Type the substring (no more than %d characters: ", MAX_LENGTH);
    fgets(substr, MAX_LENGTH, stdin);
    
    
    
    printf("Not found\n");
    return 0;
}
