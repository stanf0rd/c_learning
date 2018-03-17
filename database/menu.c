#include "tree.h"
#include "print.h"
#include "menu.h"
#include <string.h>

void bookMenu() {
	printf("\n***Работа с книгами***");
	char *ISBN = readData(1);
	if (ISBN == NULL) {mainMenu(); return;}
	printf("\nПоиск книги в базе данных...\n");
	bookData *book = findBook(booksRoot, ISBN);
	if (book == NULL) {addNewBook(ISBN); return;}
	printf("Книга найдена.\n\n");
	printf("ISBN: %s\n", ISBN);
	printBook(book);
	printf("\nДоступные действия:\n");
	printf("1 - Удалить эту книгу\n");
	printf("2 - Выдать эту книгу\n");
	printf("3 - Принять эту книгу\n");
	printf("4 - Вывести информацию по студентам, взявшим эту книгу\n");
	if (admin == 1) printf("0 - Выйти в главное меню\n");
	else printf("5 - Завершить работу\n");
	printf("Введите выбор: ");
	int c;
	scanf("%d", &c);
	while (getchar() != '\n');
	switch (c) {
		case 1:
			if (book->next == NULL) {
				booksRoot = delete(booksRoot, ISBN);
				printf("Книга успешно удалена.\n");
				printTime();
				fprintf(logfile, "%s deleted book with ISBN %s\n", user, ISBN); }
			else {
				printf("Эта книга есть на руках у студентов.\n");
				printTime();
				fprintf(logfile, "%s tried to delete book with ISBN %s\n", user, ISBN); }
			break;
		case 2:
			if (checkBook(ISBN) == 0) {bookMenu(); return;}
			char *ID = readData(10);
			if (ID == NULL) {bookMenu(); return;}
			giveBook(ID, ISBN);
			free(ID);
			break;
		case 3:
			ID = readData(11);
			if (ID == NULL) {bookMenu(); return;}
			returnBook(ID, ISBN);
			free(ID);
			break;
		case 4:
			printWhoTook(book);
			break;
		case 5:
			free(ISBN);
			if (admin == 1) bookMenu();
			else saveAndExit();
			break;
		case 0:
			free(ISBN);
			if (admin == 1) mainMenu();
			else bookMenu();
			break;
	}
	free(ISBN);
	bookMenu();
}

void studentMenu() {
	printf("\n***Работа со студентами***");
	char *ID = readData(2);
	if (ID == NULL) {mainMenu(); return;}
	printf("\nПоиск студента в базе данных...\n");
	studentData *student = findStudent(studentsRoot, ID);
	if (student == NULL) {addNewStudent(ID); return;}
	printf("Студент найден.\n\n");
	printf("Номер зачётки: %s\n", ID);
	printStudent(student);
	printf("\nДоступные действия:\n");
	printf("1 - Удалить этого студента\n");
	printf("2 - Редактировать информацию по этому студенту\n");
	printf("3 - Вывести информацию по взятым этим студентом книгам\n");
	if (admin == 1) printf("0 - Выйти в главное меню\n");
	else printf("4 - Завершить работу\n");
	printf("Введите выбор: ");
	int c;
	scanf("%d", &c);
	while (getchar() != '\n');
	switch (c) {
		case 1:
			if (student->next == NULL) {
				studentsRoot = delete(studentsRoot, ID);
				printf("Студент успешно удалён.\n"); }
			else printf("На руках у этого студента есть книги.\n");
			break;
		case 2: editStudent(student); break;
		case 3: printHisBooks(student); break;
		case 4:
			free(ID);
			if (admin == 1) studentMenu();
			else saveAndExit();
			break;
		case 0:
			free(ID);
			if (admin == 1) mainMenu();
			else studentMenu();
			break;
	}
	free(ID);
	studentMenu(0);
}

void mainMenu() {
	printf("\nГлавное меню:\n");
	printf("1 - Книги\n");
	printf("2 - Студенты\n");
	printf("3 - Закончить работу\n");
	printf("Введите выбор: ");
	int c;
	scanf("%d", &c);
	while (getchar() != '\n');
	switch (c) {
		case 1: bookMenu(); break;
		case 2: studentMenu(); break;
		case 3: saveAndExit();
	}
	mainMenu();
}

void addNewBook(char *ISBN) {
	printf("Книга с ISBN %s не найдена.\n", ISBN);
	printf("1 - Добавить\n");
	printf("0 - Выбрать другую книгу\n");
	int c;
	printf("Введите выбор: ");
	scanf("%d", &c);
	while (getchar() != '\n');
	if (c==0) {bookMenu(); return;}
	else if (c==1) {
		printf("\n");
		char *title = NULL, *author = NULL;
		while (title == NULL) title = readData(3);
		while (author == NULL) author = readData(4);
		bookData *book = malloc(sizeof(bookData));
		book->ISBN = ISBN;
		book->title = title;
		book->author = author;
		printf("Введите количество экземпляров книги: ");
		while (scanf("%d", &(book->count)) != 1);
		while (getchar() != '\n');
		book->taken = book->count;
		book->next = NULL;
		addBook(booksRoot, book);
		printTime();
		fprintf(logfile, "%s created new book with ISBN %s\n", user, ISBN);
	}
	else {printf("\nНеверный ввод.\n"); addNewBook(ISBN); return;}
}

void addNewStudent(char *ID) {
	printf("Студент с номером зачётки %s не найден.\n", ID);
	printf("1 - Добавить\n");
	printf("0 - Выбрать другого студента\n");
	int c;
	printf("Введите выбор: ");
	scanf("%d", &c);
	while (getchar() != '\n');
	if (c==0) {studentMenu(); return;}
	else if (c==1) {
		printf("\n");
		char *surname = NULL, *name = NULL, *fathername = NULL, *faculty = NULL, *specialty = NULL;
		while (surname == NULL) surname = readData(5);
		while (name == NULL) name = readData(6);
		while (fathername == NULL) fathername = readData(7);
		while (faculty == NULL) faculty = readData(8);
		while (specialty == NULL) specialty = readData(9);
		studentData *student = malloc(sizeof(studentData));
		student->ID = ID;
		student->surname = surname;
		student->name = name;
		student->fathername = fathername;
		student->faculty = faculty;
		student->specialty = specialty;
		student->next = NULL;
		addStudent(studentsRoot, student);
		printTime();
		fprintf(logfile, "%s created new student with ID %s\n", user, ID);
	}
	else {addNewStudent(ID); return;}
}

void editStudent(studentData *student) {
	printf("\nРедактирование данных студента.\n");
	char *surname = NULL, *name = NULL, *fathername = NULL, *faculty = NULL, *specialty = NULL;
	while (surname == NULL) surname = readData(5);
	while (name == NULL) name = readData(6);
	while (fathername == NULL) fathername = readData(7);
	while (faculty == NULL) faculty = readData(8);
	while (specialty == NULL) specialty = readData(9);
	student->surname = surname;
	student->name = name;
	student->fathername = fathername;
	student->faculty = faculty;
	student->specialty = specialty;
	printf("Данные студента успешно изменены.\n");
	printTime();
	fprintf(logfile, "%s edited student with ID %s\n", user, student->ID);
}

void saveAndExit() {
	FILE *studFile = fopen("csv/students.csv", "w");
	FILE *bookFile = fopen("csv/books.csv", "w");
	FILE *takenFile = fopen("csv/student_books.csv", "w");
	if (studFile!=NULL && bookFile != NULL && takenFile != NULL) {
		saveStudents(studentsRoot, studFile);
		saveBooks(booksRoot, bookFile);
		printStudentsBooks(studentsRoot, takenFile);
		fclose(studFile);
		fclose(bookFile);
		fclose(takenFile); }
	else report(7);
	freeAllStudents(studentsRoot);
	freeAllBooks(booksRoot);
	printTime();
	fprintf(logfile, "%s saved all data and exited\n\n", user);
	fclose(logfile);
	exit(0);
}
