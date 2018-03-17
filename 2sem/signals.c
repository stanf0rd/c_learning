#include <stdio.h>

int main()
{
    int c;
    while((c=getchar()) != EOF)
    {
        if (c=='\n') printf("\\n");
        else if (c=='\t') printf("\\t");
        else if (c=='\a') printf("\\a");
        else if (c=='\b') printf("\\b");
        else if (c=='\f') printf("\\f");
        else if (c=='\r') printf("\\r");
        else if (c=='\v') printf("\\v");
        else putchar(c);
    }
    printf("\n");
}