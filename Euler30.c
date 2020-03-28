/*
Surprisingly there are only three numbers that can be written as the sum of fourth powers of their digits:

1634 = 1^4 + 6^4 + 3^4 + 4^4
8208 = 8^4 + 2^4 + 0^4 + 8^4
9474 = 9^4 + 4^4 + 7^4 + 4^4
As 1 = 1^4 is not a sum it is not included.

The sum of these numbers is 1634 + 8208 + 9474 = 19316.

Find the sum of all the numbers that can be written as the sum of fifth powers of their digits.

Answer: 443839
*/
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

int32_t main()
{
	uint64_t answer = 0, sum_powers, i, n, d;

	for (i = 2; i < 354295; ++i)
	{
		n = i;
		sum_powers = 0;

		while (n > 0)
		{
			d = n%10;
			n /= 10;
			d = d*d*d*d*d;
			sum_powers += d;
		}

		if (sum_powers == i)
			answer += i;
	}

	printf("Result: %" PRIu64 "\n", answer);
	return 0;
}