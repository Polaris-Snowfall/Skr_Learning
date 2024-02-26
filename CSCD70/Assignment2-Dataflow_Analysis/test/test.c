#include <stdio.h>

void foo(int A,int B)
{
    int C,D;
    C = A+B;
    if(A)
    {
        A = A+1;
    }
    D = A+B;
    printf("%d%d",C,D);
}