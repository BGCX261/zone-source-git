#include <stdio.h>

/* /\* compile error *\/ */
/* void modify_pointer_content_0(const int *array) */
/* { */
/* 	*array = 1; */
/* } */

/* /\* compile error *\/ */
/* void modify_pointer_content_1(const int *array) */
/* { */
/* 	array[1] = 1; */
/* } */

/* /\* compile error *\/ */
/* void modify_pointer_content_10(const int *array) */
/* { */
/* 	array[10] = 1; */
/* } */


/* compile error */
void modify_pointer_content_10(int const array[10])
{
	int const *a = array+1;
	int *b;

	b = a+1;
	/* a = array+1; */
	/* a = 1; */
}


int
main(void)
{
	int a[10];

	modify_pointer_content_10(a);

	return 1;
}
