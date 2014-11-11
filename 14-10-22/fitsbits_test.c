#include <stdio.h>

int fitsBits1(int x, int n) {
	printf("%d -> %d %d\n", x, (x & ((1 << (n - 1)) - 1)) |
			(!!(x >> 31) << (n - 1)), x & ((1 << n) - 1));
	return !(
		(
			(x & ((1 << (n - 1)) - 1)) |
			(!!(x >> 31) << (n - 1))
		) +
		(
			~(x & ((1 << n) - 1)) + 1
		)
	);
}

int fitsBits(int x, int n) {
	int s = ~((1 << n) - 1);
	int t = x & (1 << (n - 1));
	// ((n & s == s) && t)     ||   (!(n & s) && t)
	// (!((n & s) ^ s) & !!t) | (!(n & s) & !!t)
	return ((!((x & s) ^ s) & !!t) | (!(x & s) & !!t));
}

int fitsBits_hack(int x, int n) {
	return (
		(x >= 0 - (1 << (n - 1))) &&
		(x <= (1 << (n - 1)) - 1)
	);
}

void test(int x_m, int n_m) {
	int x, n;
	for (x = 0-x_m; x <= x_m; ++x)
		for (n = 2; n <= n_m; ++n) {
			printf("%d %d\n", x, n);
			if (fitsBits_hack(x, n) != fitsBits(x, n)) {
				printf("Fail: %d %d (%d vs. %d)\n",
					x,
					n,
					fitsBits_hack(x, n),
					fitsBits(x, n)
				);
				return;
			}
		}
	printf("OK\n");
}

int main(void) {
	int x, n;
	printf("Type X and N: ");
	scanf("%d%d", &x, &n);
	test(x, n);
	/* printf("%d %d\n%d\n%d\n",
		0 - (1 << (n - 1)),
		(1 << (n - 1)) - 1,
		fitsBits_hack(x, n),
		fitsBits(x, n)
	); */
	return 0;
}
