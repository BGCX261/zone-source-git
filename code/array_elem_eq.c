/**
 * @Function    :  elem_eq
 * @Description :  estimate all the elements of array are the same or not.
 * @Parameter   :  array - array
 *                 len - the len of array
 *                 n - the num of bytes of each element
 * @Return      :  1 - the same
 *                 0 - not the same
 */
int elem_eq(void *array, unsigned int len, unsigned int n)
{
	int i, j;
	
	for (i = 0; i < len-1; i++)
		for (j = 0; j < n; j++)
			if (*(char *)(array+i*n+j) != *(char *)(array+(i+1)*n+j))
				return 0;

	return 1;
}
