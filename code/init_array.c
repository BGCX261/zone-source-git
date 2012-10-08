/**
 * @p: 是否可以在编译时用数组的元素初始化数组的元素。
 **/
#include <stdio.h>

char a[]= {'a','b','c'};
/* char b[]= {'d','e',a[1]}; 错误*/

int main(void)
{
	char b[]= {'d','e',a[1]};

	printf("b[2] = %c\n", b[2]);
	return 0;
}
