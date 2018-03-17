#pragma once
#include "tree.h"

void closeWithError(int);
void report(int);
void printStudent(studentData *);
void printBook(bookData *);
void saveStudents(node *, FILE *);
void saveBooks(node *, FILE *);
void printWhoTook(bookData *);
void printHisBooks(studentData *);
void printTime();
int dateToDays(char *);
char *readData(int);
void printStudentsBooks(node *, FILE *);
