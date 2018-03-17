//работает со строками и файлами
//при работе не раздражает пустыми строками в терминале
//адекватно воспринимает пробелы, табуляцию и перенос строк в любой части строки/файла

#include <stdio.h>

int main()
{
    int c, spaces=0, enters=1;
    while ((c = getchar()) != EOF)
    {
        if (c=='\n')
        {
            if (enters==0) printf("\n");
            enters=1;
        }
        else if ((c==' ') || (c=='\t') || (c==194) || (c==160)) spaces=1;
        else
        {
            if ((spaces>0)&&(enters==0)) printf("\n");
            spaces=0;
            enters=0;
            putchar(c);
        }
    }
}