//возможны два режима - на 40 и на 200 символов в строке
//есть предупреждение о превышении числа символов в строке
//можно было без всего этого, а также аргументов, malloc, fgets и в 20 строк - но так неинтересно 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEL_CHARS 40 //max. amount of deleting chars

void error(int key)
{
        if (key==1) printf("Too much keys.\n");
        if (key==2) printf("Invalid key. Try to use -xl key.\n");
        exit(key);
}

void main(int argc, char *argv[])
{
	int str_size, c, nochar=0;
	char *line, *delete_chars;
	if (argc>2) error(1);
	if (argc==1)
	{
		str_size = 40;
		printf("Started in normal mode. Maximum  10 symbols. For 200-symbols-mode use -xl key.\n");
	}
	else
	{
		if (strcmp(argv[1], "-xl")!=0) error(2);
		str_size = 200;
		printf("Started in xl mode. Maximum 200 symbols.\n");
	}
	line = malloc((str_size+3)*sizeof(char)); //3: '\n', '\0', checksymbol
	delete_chars = malloc((DEL_CHARS+3)*sizeof(char));

	printf("Type in a string:\n");
	fgets(line, str_size+3, stdin);
	if (strlen(line)>str_size+1)
	{
		printf("Too much symbols. Only first %d ones will be checked.\n", str_size);
		while (c!='\n') c=getchar();
	}
	printf("Type symbols that need to be deleted. Maximum %d symbols:\n", DEL_CHARS);
	fgets(delete_chars, DEL_CHARS+3, stdin);
	if (strlen(delete_chars)>(DEL_CHARS+1))
		printf("Too much symbols. Only first %d ones will be deleted.\n", DEL_CHARS);
	for (int i=0; ((i<strlen(line)-1) || (i<str_size)); i++)
	{
		for (int j=0; ((j<strlen(delete_chars)-1) || (j<DEL_CHARS)); j++)
			if (*(line+i) == *(delete_chars+j)) nochar=1;
		if (nochar==0) printf("%c", *(line+i));
		else nochar=0;
	}
	putchar('\n');
}