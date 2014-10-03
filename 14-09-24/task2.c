#include <stdio.h>

unsigned long long toBinary(unsigned long num10) {
	int mult = 1;
	unsigned long long num2 = 0;
	while (num10) {
		num2 += mult * (num10 & 1);
		num10 >>= 1;
		mult *= 10;
	}
	return num2;
}

int main(void) {
	printf("Type a decimal number: ");
	unsigned long num10;
	scanf("%lu", &num10);
	printf("%llu\n", toBinary(num10));
	return 0;
}
