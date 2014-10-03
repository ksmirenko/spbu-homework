#include <stdio.h>

int fitsBits(int x, int n) {
	int s = ~((1 << n) - 1);
	int t = x & (1 << (n - 1));
	// ((n & s == s) && t)     ||   (!(n & s) && t)
	// (!((n & s) ^ s) & !!t) | (!(n & s) & !!t)
	return ((!((x & s) ^ s) & !!t) | (!(x & s) & !!t));
}

int main(void) {
	int x, n;
	printf("Type X and N: ");
	scanf("%d%d", &x, &n);
	printf("%d\n", fitsBits(x, n));
	return 0;
}
