#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct takenByStudent {
	struct studentData *student;
	char *date;
	struct takenByStudent *next;
} takenByStudent;

typedef struct bookData {
	char *ISBN;
	char *title;
	char *author;
	int count;
	int taken;
	takenByStudent *next;
} bookData;

typedef struct takenBook {
	struct bookData *book;
	char *date;
	struct takenBook *next;
} takenBook;

typedef struct studentData {
	char *ID;
	char *surname;
	char *name;
	char *fathername;
	char *faculty;
	char *specialty;
	takenBook *next;
} studentData;

typedef struct node {
	bookData *bookDataPnt;
	studentData *studentDataPnt;
	unsigned char height;
	struct node *left;
	struct node *right;
} node;

char *user;
FILE *logfile;
node* studentsRoot;
node* booksRoot;

studentData* scanStudent (FILE *, FILE *);
bookData* scanBook (FILE*, FILE*);
node* addStudent (node*, studentData*);
node* addBook (node*, bookData*);
node *readStudentsFile();
node *readBooksFile();
node* delete(node*, char *);
studentData *findStudent(node *, char *);
bookData *findBook(node *, char *);
void readAllTaken();
void freeStudentData(studentData *);
void freeBookData(bookData *);
void freeAllStudents(node *);
void freeAllBooks(node *);
void giveBook(char *, char *);
void editStudent();
int checkBook(char *);
void returnBook(char *, char *);
