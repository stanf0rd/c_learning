#include <stdio.h>

int main()
{
    int i,j;
    for(i=1; i<10; i++)
    {
        for(j=1; j<10; j++)
        {
            printf("%dx%d=%d", j, i, j*i);
            if (i*j<10) printf(" ");
            printf("   ");
            if (j==9)
            {
                printf("\n");
            }
        }
    }
    return 0;
}