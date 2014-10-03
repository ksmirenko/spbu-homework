#include <stdio.h>

unsigned long pow2(unsigned long a, unsigned long k) {
	if (!k)
		return 1;
	unsigned long t = pow2(a, k >> 1);
	return (k & 1 ? t * t * a : t * t); 
}

int main(void) {
	printf("Type the base and the exponent: ");
	unsigned long b, exp;
	scanf("%lu%lu", &b, &exp);
	printf("%lu\n", pow2(b, exp));
	return 0;
}

