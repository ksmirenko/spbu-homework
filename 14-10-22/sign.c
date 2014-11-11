/*
    Task: Print the sign of a number:
        num > 0  ->  1
        num == 0 ->  0
        num < 0  -> -1
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>

int main(void) {
	int n;
	printf("Type a number: ");
	scanf("%d", &n);
	printf("%d\n", (n >> 0x1F) | (!!n));
	return 0;
}
