#include <stdio.h>
#include <string.h>

int func(int c, int count)
{
    if (c==1) printf("%d spaces\n", count);
    if (c==2) printf("%d tabs\n", count);
    if (c==3) printf("%d lines\n", count);
}

int man()
{
    printf("\n      Manual\n-s: count spaces\n-t: count tabs\n-e: count enters\nStart programm without arguments to open Manual\n\n");
}

int main(int argc, char * argv[])
{
    int i, ch;
    int spaces=0, tabs=0, enters=0;
    int wasspace=0, wastab=0, wasenter=0;
    if (argc==1) man();
    else
    {
        while ((ch=getchar()) != EOF)
        {
            if (ch==' ') spaces++;
            if (ch=='\t') tabs++;
            if (ch=='\n') enters++;
        }
        for(i=1; i<argc; i++)
        {
            if ((strcmp(argv[i],"-s")==0)&&(wasspace==0)) {func(1, spaces); wasspace=1;}
            else if ((strcmp(argv[i],"-t")==0)&&(wastab==0)) {func(2, tabs); wastab=1;}
            else if ((strcmp(argv[i],"-e")==0)&&(wasenter==0)) {func(3, enters); wasenter=1;}
        }
    }
}