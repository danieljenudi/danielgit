#include <stdio.h>

void func2(void *arg)
{
    long f2;
    printf("Enter func2\n");

    /* (2) */

    printf("Exit func2\n");
}

void func1(void)
{
    long f1;
    printf("Enter func1\n");
    func2(&f1);
    printf("Exit func1\n");
}

int main(void)
{
    const long x = 1;
    long y = 5;

    /* (1) */

    printf("x = %lu\n", x);
    func1();

    printf("Exit main\n");
    return 0;
}