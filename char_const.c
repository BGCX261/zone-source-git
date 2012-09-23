/***********************************************************
File Name : char_const.c
Description : 
************************************************************/
#include <stdio.h>

int
fun1(char **result)
{
    char char_const[] = "ABCDE"; /* 即使是常量，在函数返回后也会被释放 */

    *result = char_const;

    return 0;
}

int
fun2(char **result)
{
    char *char_const = "ABCDE";

    *result = char_const;

    return 0;
}

char *
fun3(void)
{
    char char_const[] = "ABCDE";

    return char_const;
}

char *
fun4(void)
{
    char *char_const = "ABCDE";
    
    return char_const;
}

char *
fun5(void)
{
    char char_const[] = {'A', 'B', 'C', 'D', 'E', '\0'};

    return char_const;
}

int
main(void)
{
    char *result = "FGH";

    fun1(&result);
    printf("fun1 : %s\n", result);

    fun2(&result);
    printf("fun2 : %s\n", result);

    printf("fun3 : %s\n", fun3());

    printf("fun4 : %s\n", fun3());

    printf("fun5 : %s\n", fun3());

    return 0;
}
