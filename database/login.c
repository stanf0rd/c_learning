#include <string.h>
#include "print.h"

static void length(int *login, int *pass) {
	FILE *file = fopen("csv/users.csv", "r");
	if (file == NULL) closeWithError(6);
	int c, cell=0, loglength=0, passlength=0;
	while ((c=fgetc(file))!=EOF) {
		if (c == ';') {
			cell++;
			if (loglength > *login) *login = loglength;
			if (passlength > *pass) *pass = passlength;
			loglength = 0;
			passlength = 0;
		}
		else if (cell == 0) loglength++;
		else if (cell == 1) passlength++;
		else {
			cell = 0;
			while (c!='\n' && c!=EOF) c=fgetc(file);
		}
	}
	fclose(file);
}

void login(int *student, int *books) {
	int c, loglength=0, passlength=0;
	length(&loglength, &passlength);
	FILE *file = fopen("csv/users.csv", "r");
	printf("Введите логин: ");
	c=getchar();
	char login[loglength+1];
	for (int i=0; i<loglength; i++) {
		if (c=='\n' || c=='\r') {login[i]='\0'; break;}
		login[i] = c;
		c=getchar();
	}
	login[loglength] = '\0';
	if (c!='\n' && c!='\r') {printf("Слишком длинный логин.\n"); return;}

	char loginInFile[loglength+1], passInFile[passlength+1];
	int diff=1, studentAccess=0, bookAccess=0;
	while (diff!=0) {
		c=fgetc(file);
		for (int i=0; i<loglength; i++) {
			if (c==';') {loginInFile[i]='\0'; break;}
			loginInFile[i] = c;
			c=fgetc(file);
		}
		login[loglength] = '\0';
		c=fgetc(file);
		for (int i=0; i<passlength; i++) {
			if (c==';') {passInFile[i]='\0'; break;}
			passInFile[i] = c;
			c=fgetc(file);
		}
		passInFile[passlength] = '\0';
		fscanf(file, "%d", &studentAccess);
		fgetc(file);
		fscanf(file, "%d", &bookAccess);
		while (c!='\n' && c!=EOF) c=fgetc(file);
		if (c == EOF) {printf("Неверный логин.\n"); return; }
		diff = strcmp(login, loginInFile);
	}
	printf("Введите пароль: ");
	c=getchar();
	char password[passlength+1];
	for (int i=0; i<passlength; i++) {
		if (c=='\n' || c=='\r') {password[i]='\0'; break;}
		password[i] = c;
		password[passlength] = '\0';
		c=getchar();
	}
	if (c!='\n' && c!='\r') {printf("Неверный пароль.\n"); return;}
	if (strcmp(password, passInFile) != 0) return;
	else {
		user = strdup(login);
		*student = studentAccess;
		*books = bookAccess;
		printf("\nДоступ к работе со студентами:");
		if (*student == 1) printf(" есть.\n");
		else printf(" закрыт.\n");
		printf("Доступ к работе с книгами:");
		if (*books == 1) printf(" есть.\n\n");
		else printf(" закрыт.\n\n");
	}
	fclose(file);
}
