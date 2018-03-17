//3-way quicksort. И partition, и 3w-qs, и swap написал сам.
//файл перед сортировкой очищается от пустых строк и аккуратно складывается в кучу
//во время сортировки перемещаются указатели, а не строки, т.к. файл лежит в массиве указателей (по указателю на строку)
//файл занимает в оперативной памяти ровно столько байт, сколько весит сам (каждое строке выделяется ровно нужное ей число байт)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START 2000 //code of a nonexistent symbol
#define END_OF_LINE line[i] + strlen(line[i])

void file_data(FILE *, int *, int *, int *);
void swap(char *[], int, int);
void partition(char *[], int, int *, int *);
void three_way_quicksort(char *[], int);
void error(int);
void man();

void main(int argc, char *argv[])
{
	FILE *file;
	if (argc==1) man();
	if (argc==2)
	{
		file = fopen(argv[1], "r");
		if (file==NULL) error(1);
	}
	else error(2);

	//counting strings, simbols and file size
	int prev=START, symb_in_str=0, str_count=0, str_size=0, file_size=0;
	file_data(file, &str_count, &str_size, &file_size);
	fclose(file);
	if (str_count<2) error(3);
	
	//memory allocating:
	char *line[str_count];
	line[0]=malloc(file_size+2*str_count);
	if (line[0]==0) printf("unable to allocate memory\n");
	char *for_free=line[0];
	
	//reopen file after counting strings & string size
	file = fopen(argv[1], "r");
	prev=START;
	for (int i=0; i<str_count; i++)
	{
		if (prev==START) prev=0;
		else line[i] = line[i-1] + strlen(line[i-1]) + 1;
		fgets(line[i], str_size+2, file);
		if (strcmp(line[i], "\n")==0) i--;
		if ((*line[i]=='\n') || (*line[i]=='\r')) i--;
		if ( *(END_OF_LINE-1) == '\n') *(END_OF_LINE-1)='\0';
	}
	
	three_way_quicksort(&line[0], str_count);
	//print array:
	for (int i=0; i<str_count; i++) printf("%d: %s\n", i+1, line[i]);
	free(for_free);
}

void file_data(FILE *file, int *str_count, int *str_size, int *file_size)
{
	int c=0, symb_in_str=0, prev=START;
	while (c!=EOF)
	{
		c=fgetc(file);
		if ((c=='\r') && (prev=='\n')) while (c!='\n') {prev=c; c=fgetc(file);}
		else if ( (c=='\n')||(c==EOF) )
		{
			if (prev!='\n') (*str_count)++;
			if (symb_in_str>*str_size) *str_size=symb_in_str;
			*file_size += symb_in_str;
			symb_in_str=0;
		}
		else symb_in_str++;
		prev=c;
	}
}

void swap(char *line[], int a, int b)
{
	char *bufer;
	bufer=line[a];
	line[a]=line[b];
	line[b]=bufer;
}

void partition(char *line[], int str_count, int *i_pnt, int *j_pnt)
{
	int pivot=str_count-1, i=pivot-1, j=0;
	while ((i+1)!=j)
	{
		if (strcmp(line[i], line[pivot]) < 0) swap(&line[0], i, j++);
		else if (strcmp(line[i], line[pivot]) > 0) swap(&line[0], i--, pivot--);
		else i--;
	}
	*i_pnt=++i;
	*j_pnt=++pivot;
}

void three_way_quicksort(char *line[], int high)
{
	if (high<=0) return;
	int i=0, j=0;
	partition(&line[0], high, &i, &j);
	high-=j;
	if (i>1) three_way_quicksort(&line[0], i);
	three_way_quicksort(&line[j], high);
}

void man()
{
	printf(" ********************************\n *    Use only one argument:    *\n");
	printf(" *       ./a.out file.txt       *\n *   ************************   *\n");
	printf(" *    To see the manual again   *\n * run the program without keys.*\n");
	printf(" ********************************\n");
	exit(0);
}

void error(int key)
{
	if (key==1) printf("Error. Invalid file path, file name or no permission.\n");
	if (key==2) printf("Error. Invalid arguments.\n");
	if (key==3) printf("Nothing to sort\n");
	exit(key);
}