#include <stdio.h>

int main(void) {
	printf("Type an int: ");
	int num10;
	scanf("%d", &num10);
	int cur = 31;
	while (!((num10 & (1 << cur)) || !cur))
		cur--;
	while (cur >= 0) {
		printf("%d", !!(num10 & (1 << cur)));
		--cur;
	}
	printf("\n");
	return 0;
}
