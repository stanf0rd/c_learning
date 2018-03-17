//написал всю хэш-таблицу сам, не трогая Кернигана/Ритчи (не считая хэш-функции) 
//от слов отдираются точки, запятые, кавычки и прочий мусор, считаются только сами слова
//функция scan_file возвращает указатель на слово в массиве, в котором ровно столько байт, сколько занимает именно это слово. Таким образом экономятся не меньше 80-90% оперативы в сравнении с выделением массива фиксированной длины для всех слов, а также программа верно сработает с любыми неограниченно большими словами
//возможность не выводить список уникальных слов после вывода их числа

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HASHSIZE 101

struct words *hashtab[HASHSIZE];
unsigned hash(char *);
char *scan_file(FILE *, FILE *);
void error(int);
void man();

struct words {
	char *name; //имя
	int count; //число повторений
	struct words *next; //указатель на следующий элемент
	int index; //номер структуры в списке
};

void main(int argc, char *argv[]) {
	
	//working with arguments, opening files, declaring variables
	if (argc==1) man();
	if (argc!=2) error(1);
	FILE *file_count = fopen(argv[1], "r"), *file_read = fopen(argv[1], "r");
	if (file_count == NULL) error(2);
	char *word_pnt;
	int word_count=0;
	struct words *struct_pnt, *struct_prev;

	while ((word_pnt = scan_file(file_count, file_read)) != NULL) {	
	//lookup	
		int i=0;
		struct_pnt = hashtab[hash(word_pnt)];
		while (struct_pnt!=NULL) {
			if (strcmp(struct_pnt->name, word_pnt) == 0) break;
			struct_prev = struct_pnt;
			struct_pnt = struct_pnt->next;
			i++; } 

	//install
		if (struct_pnt!=NULL) struct_pnt->count++;
		else {
			struct_pnt = (struct words *)malloc(sizeof(struct_pnt));
			struct_pnt->name = strdup(word_pnt);
			struct_pnt->count = 1;
			if (i==0) hashtab[hash(word_pnt)] = struct_pnt;
			else struct_prev->next = struct_pnt; 
			word_count++;}
	}

	//printing table
	printf("File scanned, hashtable created.\n%d unique words counted. ", word_count);
	int c=0;
	while(c!='y' && c!='n') {
		printf("Print the table? (y/n) ");
		c=getchar(); 
		if (c=='y') {
			for (int i=0; i<HASHSIZE; i++) {
				struct_pnt = hashtab[i];
				while (struct_pnt != NULL) {
					printf("%-20s    %7d\n", struct_pnt->name, struct_pnt->count);
					struct_pnt = struct_pnt->next; }
			}
		}
	}
	exit(0);
}

unsigned hash(char *s) {
	unsigned hash;
	for (hash=0; *s!='\0'; s++) 
		hash = *s + 31*hash;
	return hash % HASHSIZE;
}

char* scan_file(FILE *file_count, FILE *file_read) {
	int i=0, c=0;
	while(c = fgetc(file_count)) {
		if ((c<'0' || c>'z')||(c>'9' && c<'A')||(c>'Z' && c<'a')) { 
			if (i==0) {if (fgetc(file_read) == EOF) return NULL;}
			else break; } 
		else i++;
	}
	char *word = malloc(i*sizeof(char));
	for (int j=0; j<=i; j++) *(word+j)=fgetc(file_read);
	*(word+i)='\0';
	return word;
}

void man() {
	printf(" ********************************\n *    Use only one argument:    *\n");
	printf(" *       ./a.out file.txt       *\n *   ************************   *\n");
	printf(" *    To see the manual again   *\n * run the program without keys.*\n");
	printf(" ********************************\n");
	exit(0);
}

void error(int key) {
	if (key==1) printf("Error. Invalid arguments.\n");
	if (key==2) printf("Error. Invalid file path, file name or no permission.\n");
	exit(key); 
}