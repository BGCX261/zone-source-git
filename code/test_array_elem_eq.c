#include <stdio.h>
#include "array_elem_eq.h"

int main(void)
{
	/* char c[3] = {'a', 'a', 'a'}; */
	char c[3] = {'a', 'a', 'a'};
	int i[5] = {10,10,10,10,10};
	float f[3] = {10.0, 10.0, 10.0};

	if (elem_eq(c, 3, 1))
	{
		printf("char eq\n");
	}

	if (elem_eq(i, 5, sizeof(int)))
	{
		printf("int eq\n");
	}

	if (elem_eq(f, 3, sizeof(float)))
	{
		printf("float eq\n");
	}
	
	return 0;
}
