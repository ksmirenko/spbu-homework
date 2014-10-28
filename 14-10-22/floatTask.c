/*
	Task: "look inside" a float number and print its value
		(numeric, Inf or NaN)
	Author: Kirill Smirenko, group 171
*/

#include <stdio.h>

// prints bit representation of an int
void printBits(int num10) {
	int cur = 31, space = 0;
	while (cur >= 0) {
		if (!space) {
			printf(" ");
		}
		printf("%d", !!(num10 & (1 << cur)));
		space = (space + 1) % 4;
		--cur;
	}
	printf("\n");
}

// prints detailed float number info
// input: 
void printFloat3(char sign, int mant, int exp) {
	// checking exceptional cases
	// zero	
	if (!mant && !exp) {
		printf("%c0\n", sign);
		return;
	}
	// Inf
	if (!mant && !(exp ^ 255)) {
		printf("%cInf\n", sign);
		return;
	}
	// NaN
	if (mant && !(exp ^ 255)) {
		printf("NaN\n", sign);
		return;
	}
	// Denormalized
	if (mant && !exp) {
		printf("Denormalized number\n", sign);
		return;
	}

	// printing mantissa
	int i = 22;
	while (!(mant & 1) && i) {
		mant >>= 1;
		--i;
	}
	printf("%c1.", sign);
	while (i >= 0) {
		printf("%d", !!(mant & (1 << i)));
		--i;
	}
	// printing exponent
	printf(" * 2^%d\n", exp - 127);
	printf("(the mantissa is represented as binary)\n");
}

// prints detailed float number info
// input: bit representation of a float
void printFloat(int ival) {
	char sign = (ival & (1 << 0x1F)) ? '-' : '+';
	int mant = ival & ((1 << 24) - 1);
	int exp = ((ival >> 23) & 255);
	printFloat3(sign, mant, exp);
}

// using pointers
void variant1(float fval) {
	int ival = *(int*)((void*) & fval);
	printFloat(ival);
	printf("\n");
}

// using union { int, float }
void variant2(float fval) {
	union {
		int iv;
		float fv;
	} un;
	un.fv = fval;
	printFloat(un.iv);
	printf("\n");
}

// using union { float, struct { bitFields } }
void variant3(float fval) {
	union {
		float fv;
		// Divide et impera
		struct {
			unsigned int mant : 23;
			unsigned int exp : 8;
			unsigned int sign : 1;
		} f;
	} un;
	un.fv = fval;
	printFloat3(un.f.sign ? '-' : '+', un.f.mant, un.f.exp);
}

int main(void) {
	float fval;
	printf("Type a float: ");
	scanf("%f", &fval);
	printf("Variant 1:\n");
	variant1(fval);
	printf("Variant 2:\n");
	variant2(fval);
	printf("Variant 3:\n");
	variant3(fval);	
}
