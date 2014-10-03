#include <stdio.h>

// num > 0  ->  1
// num == 0 ->  0
// num < 0  -> -1
int main(void) {
	int n;
	printf("Type a number: ");
	scanf("%d", &n);
	printf("%d\n", (n >> 0x1F) | (!!n));
	return 0;
}
