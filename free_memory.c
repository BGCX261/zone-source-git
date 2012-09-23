#include <stdio.h>
#include <malloc.h>
#include <string.h>

int
alloc_memory(char **p, int size)
{
    *p = (char *)malloc(size);

    return 0;
}

int
main(void)
{
    char *tmp;

    alloc_memory(&tmp, 100);

    strcpy(tmp, "ABCD");
    
    free(tmp);

    if (tmp != NULL)
    {
        strcpy(tmp, "EFG");
    }

    printf("%s\n", tmp);        /* 结果是EGF, 与char_const.c不一样 */

    return 0;
}
