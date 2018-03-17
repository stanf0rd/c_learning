#include "print.h" //tree.h included here
#include "login.c"
#include "menu.h"

void main() {
	logfile = fopen("library.log", "a");
	admin = 0;
	int i=0, studentAccess=2, bookAccess=2;
	while (studentAccess == 2) {
		login(&studentAccess, &bookAccess);
		if ((i++)>1) closeWithError(7); }
	studentsRoot = readStudentsFile();
	printTime();
	fprintf(logfile, "%s logged in\n", user);
	printf("Студенты прочтены.\n");
	booksRoot = readBooksFile();
	printf("Книги прочтены.\n");
	readAllTaken(booksRoot, studentsRoot);
	printf("Информация о взятых книгах прочтена.\n");
	if (studentAccess==1 && bookAccess==1) {admin=1; mainMenu();}
	else if (studentAccess==1) studentMenu();
	else if (bookAccess==1) bookMenu();
	saveAndExit();
	exit(0);
}
