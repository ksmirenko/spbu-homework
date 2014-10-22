#include <stdio.h>

int main(void) {
	printf("Type an int: ");
	int num10;
	scanf("%d", &num10);
	int cur, pivot = 1;
	for (cur = 31; cur >= 0; --cur) {
		if ((num10 & (1 << cur)) || (!cur))
			pivot = 0;
		if (!pivot)
			printf("%d", !!(num10 & (1 << cur)));
	}
	printf("\n");
	return 0;
}
