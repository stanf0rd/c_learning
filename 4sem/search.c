#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int string_end(char *, char *);
void error(int);
void man();

void main(int argc, char *argv[]) {
	FILE *file;
	if (argc==1) man();
	if (argc==2) {
		file = fopen(argv[1], "r");
		if (file==NULL) error(1); }
	else man();

	//counting max string size
	int c=0, str_size=0, symb_in_str=0;
	while ( (c=fgetc(file)) != EOF) {
		if ((c=='\n') || (c==EOF)) {
			if (symb_in_str>str_size) str_size=symb_in_str;
			symb_in_str=0; }
		else symb_in_str++;
	}
	fclose(file);

	//reading template avoiding buffer overflow
	char checking_line[str_size+2];
	char template[str_size+2];
	printf("Type in your search template:\n");
	int i;
	for (i=0; i<=str_size; i++) {
		c=getchar();
		template[i]=c;
		if ((c=='\n') || (c==EOF)) break; }
	if ((i==str_size+1) && (template[str_size+1] != '\n')) error(2);

	//checking file lines for template
	file = fopen(argv[1], "r");	/*reopen file after counting max string size*/
	while (fgets(checking_line, str_size+1, file) != NULL) {
		if (string_end(&template[0], &checking_line[0]) == 1) printf("%s", checking_line); }
	exit(0);
}

int string_end(char *template, char *checking_line) {
	char *pnt = &checking_line[strlen(checking_line)];
	*(pnt-2) = '\n'; /*avoiding \r except \n in file*/
	*(pnt-1) = '\0';
	int differ = strlen(checking_line)-strlen(template);
	if (differ<0) return 0;
	if (strcmp(&checking_line[differ], template) != 0) return 0;
	return 1;
}

void man() {
	printf(" ********************************\n *    Use only one argument:    *\n");
	printf(" *       ./a.out file.txt       *\n ********************************\n");
	exit(0);
}

void error(int key) {
	if (key==1) printf("Error. Invalid file path, file name or no permission.\n");
	if (key==2) printf("Error. Template is bigger than the biggest line in file.\n");
	exit(key);
}