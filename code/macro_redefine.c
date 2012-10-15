#include <stdio.h>

#ifdef a
#define a 1
#else
#define a 2
#endif

int main(void)
{
	printf("a=%d\n", a);
	return;
}
