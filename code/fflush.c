#include <stdio.h>
#include <unistd.h>

int main(void)
{
	FILE *fp = NULL;
	int i=0;

	fp = fopen("test.dat", "w");

	while(1)
	{
		sleep(1);
		fprintf(fp, "%10d\n", i++);
		printf("write\n");
		fflush(fp);
	}
	
	fclose(fp);

	return 0;
}
