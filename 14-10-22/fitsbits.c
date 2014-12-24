/*
    Task: print 1 if X can 'fit' into N bits (two's complement)
        or print 0, if otherwise.
    Author: Kirill Smirenko, group 171
*/

#include <stdio.h>

int fitsBits(int x, int n) {
	return !((x >> (n - 1)) + !!(x >> (n - 1)));
}

int main(void) {
	int x, n;
	printf("Type X and N: ");
	scanf("%d%d", &x, &n);
	printf("%d\n", fitsBits(x, n));
	return 0;
}
