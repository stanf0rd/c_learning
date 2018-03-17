#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "print.h"
#include "menu.h"
#include <time.h>

void closeWithError(int key) {
	if (key == 1) printf("Файл 'students.csv' не найден.\n");
	if (key == 2) printf("Файл 'books.csv' не найден.\n");
	if (key == 3) printf("Файл 'students.csv' повреждён.\n");
	if (key == 4) printf("Файл 'books.csv' повреждён.\n");
	if (key == 5) printf("Файл 'student_books.csv' повреждён.\n");
	if (key == 6) printf("Файл 'users.csv' повреждён.\n");
	if (key == 7) printf("Трёхкратный неверный ввод пароля или логина. Программа будет закрыта.\n");
	if (key == 9) printf("Файл 'student_books.csv' не найден.\n");
	exit(key);
}

void report(int key) {
	if (key == 1) printf("Студент с таким номером зачётки уже есть в базе.\n");
	if (key == 2) printf("Книга с таким ISBN уже есть в базе.\n");
	if (key == 3) printf("Студент с таким номером зачётки не найден.\n");
	if (key == 4) printf("Книга с таким ISBN не найдена.\n");
	if (key == 5) printf("\nКниг с таким ISBN на руках у студентов нет.\n");
	if (key == 6) printf("\nУ студента с таким ISBN на руках нет книг.\n");
	if (key == 7) printf("Нет прав на запись измененных файлов. Изменения не будут сохранены.\n");
	if (key == 8) printf("Этой книги нет на руках у этого студента.\n");
}

void printStudent(studentData* student) {
	printf("Фамилия: %s\n", student->surname);
	printf("Имя: %s\n", student->name);
	printf("Отчество: %s\n", student->fathername);
	printf("Факультет: %s\n", student->faculty);
	printf("Специальность: %s\n", student->specialty);
}

void printBook(bookData* book) {
	printf("Название: %s\n", book->title);
	printf("Автор: %s\n", book->author);
	printf("Всего в библиотеке: %d\n", book->count);
	printf("Доступно к выдаче: %d\n", book->taken);
}

void printStudentToCSV(studentData* student, FILE *file) {
	fprintf(file,
		"%s;%s;%s;%s;%s;%s\n",
		student->ID, student->surname, student->name,
		student->fathername, student->faculty, student->specialty);
}

void printBookToCSV(bookData* book, FILE *file) {
	fprintf(file,
		"%s;%s;%s;%d;%d\n",
		book->ISBN, book->title, book->author,
		book->count, book->taken);
}

void saveStudents(node *nodePointer, FILE *file) {
	if (nodePointer != NULL) {
		saveStudents(nodePointer->left, file);
		printStudentToCSV(nodePointer->studentDataPnt, file);
		saveStudents(nodePointer->right, file); }
}

void saveBooks(node *nodePointer, FILE *file) {
	if (nodePointer != NULL) {
		saveBooks(nodePointer->left, file);
		printBookToCSV(nodePointer->bookDataPnt, file);
		saveBooks(nodePointer->right, file); }
}

void printWhoTook(bookData *book) {
	takenByStudent *nextStudent = book->next;
	if (nextStudent == NULL) report(5);
	else {
		printTime();
		fprintf(logfile, "%s printed who took book with ISBN %s\n", user, book->ISBN);
		printf("\nКнигу взяли следующие(й) студент(ы):\n");
		while (nextStudent != NULL) {
			printf("%s %s %s (номер зачётки %s) до %s\n",
				nextStudent->student->surname,
				nextStudent->student->name,
				nextStudent->student->fathername,
				nextStudent->student->ID,
				nextStudent->date);
			nextStudent = nextStudent->next;
		}
	}
}

void printHisBooks(studentData *student) {
	takenBook *nextBook = student->next;
	if (nextBook == NULL) report(6);
	else {
		printTime();
		fprintf(logfile, "%s printed books taken by student with ID %s\n", user, student->ID);
		printf("\nУ студента на руках следующие книги:\n");
		while (nextBook != NULL) {
			printf("ISBN: %s, название: %s, автор: %s, взято до %s\n",
				nextBook->book->ISBN,
				nextBook->book->title,
				nextBook->book->author,
				nextBook->date);
			nextBook = nextBook->next;
		}
	}
}

void printTime() {
	time_t t = time(NULL);
  	struct tm* aTm = localtime(&t);
	fprintf(logfile, "%04d/%02d/%02d %02d:%02d:%02d ",
		aTm->tm_year+1900, aTm->tm_mon+1, aTm->tm_mday,
		aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
}

int dateToDays(char *date) {
	int number=0, month=0, year=0;
	while ((number=atoi(date++)) == 0);
	date += 2;
	while ((month=atoi(date++)) == 0);
	date += 2;
	while ((year=atoi(date++)) == 0);
	return number + month*30 + (year-1990)*365;
}

char *readData(int key) {
	int symbols;
	switch (key) {
		case 1:
			printf("\nВведите ISBN добавляемой или уже внесённой в базу книги.\n");
			symbols = 15;
			break;
		case 2:
			printf("\nВведите номер зачётки добавляемого или уже внесённого в базу студента.\n");
			symbols = 15;
			break;
		case 3:
			printf("Введите название книги.\n");
			symbols = 30;
			break;
		case 4:
			printf("Введите автора книги.\n");
			symbols = 15;
			break;
		case 5:
			printf("Введите фамилию студента.\n");
			symbols = 15;
			break;
		case 6:
			printf("Введите имя студента.\n");
			symbols = 15;
			break;
		case 7:
			printf("Введите отчество студента.\n");
			symbols = 15;
			break;
		case 8:
			printf("Введите аббревиатуру факультета.\n");
			symbols = 5;
			break;
		case 9:
			printf("Введите специальность.\n");
			symbols = 40;
			break;
		case 10:
			printf("\nДля выдачи книги студенту введите номер его зачётки.\n");
			symbols = 15;
			break;
		case 11:
			printf("\nДля сдачи книги студентом введите номер его зачётки.\n");
			symbols = 15;
			break;
		case 12:
			printf("Введите дату сдачи книги в формате dd.mm.year\n");
			symbols = 15;
			break;
	}
	if (key==1 || key==2) {
		printf("Нажатие на enter без ввода символов");
		if (admin == 1) printf(" вернёт в меню.\n");
		else printf(" сохранит данные и завершит программу.\n"); }
	printf("Максимум %d символов: ", symbols);
	char *newKeyData = malloc((2*symbols+1)*sizeof(char));
	for (int i=0; i<=(symbols*2); i++) {
		*(newKeyData+i) = getchar();
		if (*(newKeyData+i) == '\n') {
			if (i == 0) goto x;
			*(newKeyData+i) = '\0';
			return newKeyData; }
	}
	printf("Слишком большая длина. Максимум - %d символов.\n", symbols);
	x: free(newKeyData);
	return NULL;
}

static void printTaken(takenBook *book, char *ID, FILE *file) {
	if (book == NULL) return;
	if (book->next != NULL) printTaken(book->next, ID, file);
	fprintf(file, "%s;%s;%s\n", book->book->ISBN, ID, book->date);
}

void printStudentsBooks(node *nodePointer, FILE *file) {
	if (nodePointer != NULL) {
		printStudentsBooks(nodePointer->left, file);
		printTaken(nodePointer->studentDataPnt->next, nodePointer->studentDataPnt->ID, file);
		printStudentsBooks(nodePointer->right, file); }
}
