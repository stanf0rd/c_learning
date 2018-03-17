//работает с файлами и ручным вводом через stdin.
//мануальчик и информативные сообщения об ошибках и кривом вводе в комплекте.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MANUAL_INPUT (file==NULL)

void error(int);
void man();
void print(int);
int read_line(int, char *);

void main(int argc, char *argv[]) {
	FILE *file=NULL;
	int line_count=10;

	//work with arguments
	if (argc==1) print(1);
	else if (argc>4) error(1);
	else for (int i=1; i<argc; i++) {
		int n=atoi(argv[i]);
		if (strcmp(argv[i], "-m")==0 || strcmp(argv[i], "--man")==0) man();
		if (n!=0) line_count=n;
		if (i<3 && strcmp(argv[i], "-f")==0) {
			file = fopen(argv[i+1], "r");
			if (file==NULL) error(2);
			else i++;
		}
	}

	//reading lines
	print(2);
	char last_lines[line_count][122];
	if MANUAL_INPUT print(3);
	int c=0, i;
	char *pnt;
	while(c!=EOF && pnt!=NULL) {
		for (i=0; i<line_count; i++) {
			if MANUAL_INPUT {
				c=read_line(121, &last_lines[i][0]);
				if (c == EOF) break; }
			else if ((pnt=fgets(last_lines[i], 122, file)) == NULL) break;
		}
	}

	//printing saved lines
	for (int j=i; j<line_count; j++) printf("%s", last_lines[j]);
	for (int j=0; j<i; j++) printf("%s", last_lines[j]);
}

int read_line(int str_size, char *array) {
	int c;
	for (int i=0; i<=str_size; i++) {
		c=getchar();
		array[i]=c;
		if ((c=='\n') || (c==EOF)) {
			array[i]='\n';
			array[i+1]='\0';
			return c; }
	}
	while ((c!='\n') && (c!=EOF)) c=getchar();
	return c;
}

void man() {
	printf(" **********************************\n *      You can choose file       *\n");
	printf(" *  & type printing lines count.  *\n *     **********************     *\n");
	printf(" *     For manual lines input     *\n *         choose no file.        *\n");
	printf(" *     **********************     *\n *    If lines count not typed,   *\n");
	printf(" * 10 last lines will be printed. *\n *     **********************     *\n");
	printf(" *       Key using examples:      *\n *     ./a.out 20 -f file.txt     *\n");
	printf(" *       ./a.out -f file.txt      *\n *           ./a.out 20           *\n");
	printf(" **********************************\n");
	exit(0);
}

void error(int key) {
	if (key==1) printf("Error. Invalid arguments.\n");
	if (key==2) printf("Error. Invalid file path, file name or no permission.\n");
	exit(key);
}

void print(int key) {
	if (key==1) printf("Program started in default mode. 10 last lines will be printed.\n");
	if (key==2) printf("Maximal line length is 120 symbols.\n To see manual use key '-m'.\n");
	if (key==3) printf("Type in lines:\n");
}