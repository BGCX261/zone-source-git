#include <stdio.h>

int
main(void)
{
    char a[10];

    printf("%s\n", a);//局部变量字符数组是未初始化的，数据不确定。

    return 0;
}
