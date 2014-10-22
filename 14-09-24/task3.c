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
