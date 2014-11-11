/*
    Task: Search the first index of a substring in a string
        (naive algorythm, good code)
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>

const int MAX_LENGTH = 50;

int firstOcc(char s[], char subs[]) {
    int i = 0, j = 0;
    while (s[i] != '\n') {
        while ((s[i] != '\n') && (s[i] != subs[0])) {
             ++i;
        }
        if (s[i] == '\n') {
            return -1;
        }
        
        j = 0;
        while ((s[i + j] != '\n') && (subs[j] != '\n') &&
            (s[i + j] == subs[j])) {
            ++j;
        }
        
        if (subs[j] == '\n') {
            return i;
        }
        if (s[i] == '\n') {
            return -1;
        }
        
        ++i;
    }
    return -1;
}

int main() {
    char str[MAX_LENGTH], substr[MAX_LENGTH];
    
    printf("Type a string (no more than %d characters: ", MAX_LENGTH);
    fgets(str, MAX_LENGTH, stdin);
    printf("Type the substring (no more than %d characters: ", MAX_LENGTH);
    fgets(substr, MAX_LENGTH, stdin);
    
    int result = firstOcc(str, substr);
    
    if (result > -1)
        printf("Index of first occurence: %d\n", result);
    else
        printf("Not found\n");
    return 0;
}
