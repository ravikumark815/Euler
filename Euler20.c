/*
n! means n × (n − 1) × ... × 3 × 2 × 1

For example, 10! = 10 × 9 × ... × 3 × 2 × 1 = 3628800,
and the sum of the digits in the number 10! is 3 + 6 + 2 + 8 + 8 + 0 + 0 = 27.

Find the sum of the digits in the number 100!

Author: https://github.com/ravikumark815

Answer: 648
*/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Multiply the number represented by ar (1 digit per cell) per n. */
void mutiply(uint8_t* ar, int32_t size, int32_t n)
{
	int32_t carry = 0, s;
	for (int32_t i = 0; i < size; ++i)
	{
		s = ar[i] * n + carry;
		ar[i] = s%10;
		carry = s/10;
	}
}

int32_t main()
{
	/* Use a large array (1 digit per cell) to simulate Big Integers.
	 * 100! < 100^100 = 10^200 so at most 200 digits are required. */
	int32_t size = 200, MAX = 100;
	uint64_t sum = 0;
	uint8_t* number = (uint8_t*) calloc(size, sizeof(uint8_t));
	number[0] = 1;

	for (int32_t i = 1; i < MAX+1; ++i)
		mutiply(number, size, i);

	while(size--)
		sum += number[size-1];

	printf("Result: %" PRIu64 "\n", sum);
	free(number);
	return 0;
}