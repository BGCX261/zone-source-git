/**
 * @d: 负数是否为真
 **/
#include <stdio.h>

int main(void)
{
    if (-1)
        printf("-1 is true\n");

    /* can find this pattern in linux kernel */
    printf("!!(-1) = 0x%x\n", !!(-1));
    printf("!!(0) = 0x%x\n", !!(0));
    printf("!!(1) = 0x%x\n", !!(1));

    printf("1 && 1 = 0x%x\n", 1 && 1);
    return 0;
}
