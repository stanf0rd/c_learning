//быстрая сортировка и бинарный поиск в комплекте.
//фича: из файла автоматически удаляются пустые строки.
//фича: для массива, в который считывается файл, автоматически подбираются размеры и выделяется ровно столько памяти, сколько надо. Впихиваются от 2 строк до Войны и Мира.
//и мануальчик.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(int key)
{
	if (key==1) printf("Error. Invalid file path, file name or no permission.\n");
	if (key==2) printf("Error. Invalid arguments.\n");
	if (key==3) printf("This string is bigger than any string in the file.\n");
	exit(key);
}

void success(int output)
{
	if (output==-1) 
	{
		printf("********************************\n*        Welcome to the        *\n*     line-in-file  search     *\n");
		printf("*            manual!           *\n*   ************************   *\n*    Use only two arguments:   *\n");
		printf("*    ./a.out -f %%filename%%     *\n*   ************************   *\n*     ./a.out < %%filename%%     *\n*           IS WRONG!          *\n");
		printf("*   ************************   *\n*    To see the manual again   *\n* run the program without keys.*\n********************************\n");
	}
	else printf("String #%d found.\n", output); 
	exit(0);
}

int compare(const void *str1, const void *str2) {return strcmp(str1, str2);}

void main(int argc, char *argv[])
{
	int i, c, str_count=0, str_size=0, file_name_arg=2;
	FILE *file;
	if (argc==1) success(-1);
	if (argc==3)
	{
		if (strcmp(argv[1],"-f")==0 || strcmp(argv[1],"--file")==0)
		{
			file = fopen(argv[2], "r");
			if (file==NULL) error(1);
		}
	}
	else error(2);
	//counting array size
	int prev='\n', symbols=0;
	while (c!=EOF)
	{
		c=fgetc(file);
		if (c=='\n')
		{
			if (prev!='\n') str_count++;
			if (symbols>str_size) str_size=symbols;
			symbols=0;
		}
		else symbols++;
		prev=c;
	}
	//reading file to the array
	char array[str_count][str_size+2];
	file = fopen(argv[file_name_arg], "r"); //reopen file after counting strings & string size
	for (i=0; i<str_count; i++)
	{
		fgets(array[i], str_size+3, file);
		if (fgets == NULL) break;
		if (strlen(array[i]) > str_count+1) while (c=fgetc(file)!='\n');
		if (array[i][0]=='\n') i--;
	} 
//Demonstration. Uncomment these 2 lines to see the massive before and after sorting.
//	for (int i=0; i<str_count; i++) printf("%d: %s", i, array[i]);
	qsort(array, str_count, (str_size+2)*sizeof(char), compare);
//	for (int i=0; i<str_count; i++) printf("%d: %s", i, array[i]);
	char find_it[str_size+1], *point_what_find;
	point_what_find=&find_it[0];
	printf("Type a string, which you want to find:\n");
	fgets(point_what_find, str_size+3, stdin);
	if (strlen(point_what_find) > str_size+1) error(3);
	//binary search algorithm
	int first=0, middle, last=str_count-1;
	while (first <= last)
	{
		middle = (first+last)/2;
		if (strcmp(point_what_find, array[middle])<0) last = middle-1;
		else if (strcmp(point_what_find, array[middle])>0) first = middle+1;
		else success(middle);
	}
	printf("Element not found.\n");
}