/************************************************************
File Name : type_char_onst.c
Description : 理解类型 const * char * const;==(1)
                      const char *const;  ==(2)
                      const * char const; ==(3)
                      const char const;   ==(4)
                      const char const *; ==(5)
                      const char *f;      ==(6)
                      char const *g;      ==(7)
                      char *const h;      ==(8)
              总结：(1),(3)不能通过编译是因为数据类型标识符(char)前不能有
                  ‘*’.若声明变量时的变量名（假设为a）前紧跟着'*'号或‘*’号
                   与变量名之间没有const修辞,则*b不能作为赋值操作语句的
                   左值,否则b不能作为左值.
Date : 2012/02/08
 ************************************************************/
#include <stdio.h>

int
main(void)
{
    char tmp = 'a';

    /* const *char * const a;      /\* 这句不能通过编译 *\/ */

    const char *const b;

    /* const *char const c;        /\* 这句不能通过编译 *\/ */

    const char const d;

    const char const *e;

    const char *f;

    char const *g;

    char *const h;

    /* b = &tmp;          /\* 编译不能通过，提示：向只读变量‘b’赋值 *\/ */
    /* *b = tmp;          /\* 编译不能通过，提示：向只读变量‘*b’赋值 *\/ */

    /* d = tmp;           /\* 编译不能通过，提示：向只读变量‘d’赋值 *\/ */

    e = &tmp;
    /* *e = tmp;          /\* 编译不能通过，提示：向只读变量‘*e’赋值 *\/ */

    f = &tmp;
    /* *f = tmp;          /\* 编译不能通过，提示：向只读变量‘*f’赋值 *\/ */

    g = &tmp;
    /* *g = tmp;          /\* 编译不能通过，提示：向只读变量‘*f’赋值 *\/ */

    /* h = &tmp;          /\* 编译不能通过，提示：向只读变量‘*e’赋值 *\/ */
    *h = tmp;

    return 0;
}
