#include <stdio.h>

int
main(void)
{
	int a = -1;
	
	float b = 2.0;

	int tr = FALSE;
	
	printf("type(unsigned int) = %d\n", (unsigned int)((typeof(unsigned int))a - (unsigned int)1));

	return -1;
}
