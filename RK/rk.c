#include <stdio.h>

int main()
{
    int c, save1=0, save2=0, save3=0, check=0;
    while((c=getchar())!=EOF)
    {
        putchar(c);
        if (c=='<')
        {
            while ((c=getchar())!='>')
            {
                check++;
                if (check<8) putchar(c);
                else if (check==8) save1=c;
                else if (check==9) save2=c;
                else if (check==10) save3=c;
                else if (check==11) {save1=save2=save3=0; printf("...");}
            }
        if (save1!=0) {putchar(save1); save1=0;}
        if (save2!=0) {putchar(save2); save2=0;}
        if (save3!=0) {putchar(save3); save3=0;}
        check=0;
        putchar(c);
        }
    }
}