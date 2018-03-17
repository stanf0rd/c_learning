//декостыляция: нет ограничений на длину строки, даже 100500 символов адекватно перенесутся в файл. 
//милый мануальчик и информативные сообщения об ошибках или кривом вводе в комплекте. 

#include <stdio.h>
#include <stdlib.h>

void error(int);
void man();

void main(int argc, char *argv[]) {

	//work with arguments, open files
	FILE *file[3];
	if (argc==1) man();
	if (argc!=4) error(1);
	file[0]=fopen(argv[1], "r");
	file[1]=fopen(argv[2], "r");
	file[2]=fopen(argv[3], "w");
	for (int i=0; i<3; i++) if (file[i] == NULL)
		printf("Error. For file #%d invalid file path, file name or no permission.\n", i+1);
	printf("Files are successfully opened.\n");

	//reading and mixing lines
	int c=0, d=0;
	while (c!=EOF || d!=EOF) {
		while(c!=EOF) {
			c=fgetc(file[0]);
			if (c!=EOF) fputc(c, file[2]);
			else fputc('\n', file[2]);
			if (c=='\n') break; }
		while(d!=EOF) {
			d=fgetc(file[1]);
			if (d!=EOF) fputc(d, file[2]);
			else fputc('\n', file[2]); 
			if (d=='\n') break; }
	}
	for (int i=0; i<3; i++) fclose(file[i]);
	printf("Success.\n");
}

void man() {
	printf(" **********************************\n *     Program needs 3 files:     *\n");
	printf(" *  ./a.out 1.file 2.file 3.file  *\n *     **********************     *\n");
	printf(" *      Strings from 1 and 2      *\n *       will be mixed in 3.      *\n");
	printf(" *     **********************     *\n *    If file 3 doesn't exist,    *\n");
	printf(" *       it will be created.      *\n *     **********************     *\n");
	printf(" *     To see the manual again    *\n *  run the program without keys. *\n");
	printf(" **********************************\n");
	exit(0);
}

void error(int key) {
	if (key==1) printf("Error. Invalid arguments.\n");
	if (key==2) printf("Error. Invalid file path, file name or no permission.\n");
	exit(key);
}