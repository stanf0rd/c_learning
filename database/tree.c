#include <string.h>
#include <time.h>
#include <ctype.h>
#include "tree.h"
#include "print.h"

studentData* scanStudent(FILE *studCount, FILE *studRead) {
	studentData *newStudent = malloc(sizeof(studentData));
	int cell=0;
	while (1) {
		int i=0, c = fgetc(studCount);
		if (c==EOF) {free(newStudent); return NULL; }
		while ((c != ';')&&(c != '\n')&&(c != EOF)&&(c != '\r')) {
			i++;
			if (c == '"') {
				while((c=fgetc(studCount)) != '"') i++;
				i++; }
			c = fgetc(studCount);
		}
		if (((c==EOF)||(c=='\n')||(c=='\r'))&&(cell!=5)) closeWithError(3);
		cell++;
		char* newCell = malloc(1+i*sizeof(char));
		for (int j=0; j < i; j++) *(newCell+j) = fgetc(studRead);
		*(newCell+i) = '\0';
		switch (cell) {
			case 1: newStudent->ID = newCell; 			break;
			case 2: newStudent->surname = newCell; 		break;
			case 3: newStudent->name = newCell; 		break;
			case 4: newStudent->fathername = newCell; 	break;
			case 5: newStudent->faculty = newCell; 		break;
			case 6: newStudent->specialty = newCell; 	break;
		}
		c = fgetc(studRead);
		if (c=='\r') {fgetc(studRead); fgetc(studCount);}
		if (cell == 6) {
			newStudent->next = NULL;
			return newStudent; }
	}
}

bookData* scanBook(FILE *bookCount, FILE *bookRead) {
	bookData *newBook = malloc(sizeof(bookData));
	int cell=0;
	while (1) {
		int i=0, c = fgetc(bookCount);
		if (c==EOF) {free(newBook); return NULL; }
		while ((c != ';')&&(c != '\n')&&(c != EOF)&&(c != '\r')) {
			i++;
			if (c == '"') {
				while((c=fgetc(bookCount)) != '"') i++;
				i++; }
			c = fgetc(bookCount);
		}
		if (((c==EOF)||(c=='\n')||(c=='\r'))&&(cell!=4)) closeWithError(4);
		cell++;
		if (cell<4) {
			char* newCell = malloc(1+i*sizeof(char));
			for (int j=0; j < i; j++) *(newCell+j) = fgetc(bookRead);
			*(newCell+i) = '\0';
			if (cell==1) newBook->ISBN = newCell;
			if (cell==2) newBook->title = newCell;
			if (cell==3) newBook->author = newCell;
		}
		else if (cell==4) fscanf(bookRead, "%d", &(newBook->count));
		else if (cell==5) fscanf(bookRead, "%d", &(newBook->taken));
		c = fgetc(bookRead);
		if (c=='\r') {fgetc(bookRead); fgetc(bookCount);}
		if (cell == 5) {
			newBook->next = NULL;
			return newBook;}
	}
}

static unsigned char height(node *p) {
	return p?p->height:0;
}

static int bfactor(node *p) {
	return height(p->right)-height(p->left);
}

static void fixheight(node *p) {
	unsigned char heightLeft = height(p->left);
	unsigned char heightRight = height(p->right);
	p->height = (heightLeft>heightRight ? heightLeft : heightRight)+1;
}

static node *rotateLeft(node *q) { //левый поворот вокруг q
	node *p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

static node *rotateRight(node *p) { //правый поворот вокруг p
	node *q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

static node *balance(node *p) { //балансировка узла p
	fixheight(p);
	if (bfactor(p) == 2) {
		if (bfactor(p->right) < 0) p->right = rotateRight(p->right);
		return rotateLeft(p); }
	if (bfactor(p) == -2) {
		if (bfactor(p->left) > 0) p->left = rotateLeft(p->left);
		return rotateRight(p); }
	return p; // балансировка не нужна
}

node* addBook (node* p, bookData *newBook) {
	if (p == NULL) {
		node *newNodePointer = malloc(sizeof(node));
		newNodePointer->bookDataPnt = newBook;
		p = newNodePointer;
		p->left = NULL;
		p->right = NULL;
		p->studentDataPnt = NULL; }
	else {
		int diff = strcmp(newBook->ISBN, p->bookDataPnt->ISBN);
		if (diff==0) {report(2);}
		else if (diff < 0) p->left = addBook(p->left, newBook);
		else p->right = addBook(p->right, newBook); }
	return balance(p);
}

node* addStudent (node *p, studentData *newStudent) {
	if (p == NULL) {
		node *newNodePointer = malloc(sizeof(node));
		newNodePointer->studentDataPnt = newStudent;
		p = newNodePointer;
		p->left = NULL;
		p->right = NULL;
		p->bookDataPnt = NULL; }
	else {
		int diff = strcmp(newStudent->ID, p->studentDataPnt->ID);
		if (diff==0) report(1);
		else if (diff < 0) p->left = addStudent(p->left, newStudent);
		else p->right = addStudent(p->right, newStudent); }
	return balance(p);
}

node *readStudentsFile() {
	FILE *studCount = fopen("csv/students.csv", "r");
	FILE *studRead = fopen("csv/students.csv", "r");
	if (studCount == NULL) closeWithError(1);
	node *studentsRoot = NULL;
	studentData *student;
	while ((student = scanStudent(studCount, studRead)) != NULL) {
		studentsRoot = addStudent(studentsRoot, student); }
	fclose(studCount);
	fclose(studRead);
	return studentsRoot;
}

node *readBooksFile() {
	FILE *bookCount = fopen("csv/books.csv", "r");
	FILE *bookRead = fopen("csv/books.csv", "r");
	if (bookCount == NULL) closeWithError(2);
	node *booksRoot = NULL;
	bookData *book;
	while ((book = scanBook(bookCount, bookRead)) != NULL)
		booksRoot = addBook(booksRoot, book);
	fclose(bookCount);
	fclose(bookRead);
	return booksRoot;
}

studentData *findStudent(node *p, char *key) {
	if (p == NULL) {/*report(3); */return NULL; }
	int diff = strcmp(key, p->studentDataPnt->ID);
	if (diff==0) return p->studentDataPnt;
	else if (diff < 0) findStudent(p->left, key);
	else findStudent(p->right, key);
}

bookData *findBook(node *p, char *key) {
	if (p == NULL) {/*report(4); */return NULL; }
	int diff = strcmp(key, p->bookDataPnt->ISBN);
	if (diff==0) return p->bookDataPnt;
	else if (diff < 0) findBook(p->left, key);
	else findBook(p->right, key);
}

static node* findmin(node* p) {// поиск узла с минимальным ключом в дереве p
	return p->left?findmin(p->left):p;
}

static node* deletemin(node* p) {// удаление узла с минимальным ключом из дерева p
	if (p->left == NULL)
		return p->right;
	p->left = deletemin(p->left);
	return balance(p);
}

node* delete(node* p, char *k) {// удаление ключа k из дерева p
	if (!p) return NULL;
	int diff = 0;
	if (p->bookDataPnt == NULL) {
		diff = strcmp(p->studentDataPnt->ID, k);
		if (p->studentDataPnt->next != NULL) return NULL;}
	else {
		diff = strcmp(p->bookDataPnt->ISBN, k);
		if (p->bookDataPnt->next != NULL) return NULL;}
	if (diff > 0) p->left = delete(p->left, k);
	else if (diff < 0) p->right = delete(p->right, k);
	else {//  k == p->key
		node* q = p->left;
		node* r = p->right;
		if (p->bookDataPnt == NULL) {
			studentData *st = p->studentDataPnt;
			free(st->ID);
			free(st->surname);
			free(st->name);
			free(st->fathername);
			free(st->faculty);
			free(st->specialty);
			free(st); }
		else {
			bookData *b = p->bookDataPnt;
			free(b->ISBN);
			free(b->title);
			free(b->author);
			free(b); }
		free(p);
		if (!r) return q;
		node* min = findmin(r);
		min->right = deletemin(r);
		min->left = q;
		return balance(min);
	}
	return balance(p);
}

char* scanTaken(FILE *takenCount, FILE *takenRead, char **ISBN, char **ID, char **date) {
	int cell=0;
	while (1) {
		int i=0, c = fgetc(takenCount);
		if (c==EOF) return NULL;
		while ((c != ';')&&(c != '\n')&&(c != EOF)&&(c != '\r')) {
			i++;
			if (c == '"') {
				while((c=fgetc(takenCount)) != '"') i++;
				i++; }
			c = fgetc(takenCount);
		}
		if (((c==EOF)||(c=='\n')||(c=='\r'))&&(cell!=2)) closeWithError(5);
		cell++;
		char *newCell = malloc(1+i*sizeof(char));
		for (int j=0; j < i; j++) *(newCell+j) = fgetc(takenRead);
		*(newCell+i) = '\0';
		switch (cell) {
			case 1: *ISBN = newCell; break;
			case 2: *ID = newCell; break;
			case 3: *date = newCell; break;
		}
		c = fgetc(takenRead);
		if (c=='\r') {fgetc(takenRead); fgetc(takenCount);}
		if (cell == 3) return *date;
	}
}

takenByStudent* findReaderPlace(takenByStudent *place) {
	while (place->next != NULL) place = place->next;
	place->next = malloc(sizeof(takenByStudent));
	return place->next;
}

takenBook* findBookPlace(takenBook *place) {
	while (place->next != NULL) place = place->next;
	place->next = malloc(sizeof(takenBook));
	return place->next;
}

void readAllTaken() {
	FILE *takenCount = fopen("csv/student_books.csv", "r");
	FILE *takenRead = fopen("csv/student_books.csv", "r");
	if (takenRead == NULL) closeWithError(9);
	char *ISBN, *ID, *date;
	studentData *student;
	bookData *book;
	takenBook *bookPlace;
	takenByStudent *readerPlace;
	while (scanTaken(takenCount, takenRead, &ISBN, &ID, &date) != NULL) {
		book = findBook(booksRoot, ISBN);
		student = findStudent(studentsRoot, ID);
		free(ISBN);
		free(ID);
		if (book!=NULL && student!=NULL) {
			if (book->next != NULL) readerPlace = findReaderPlace(book->next);
			else {
	 			readerPlace = malloc(sizeof(takenByStudent));
				book->next = readerPlace; }
			if (student->next != NULL) bookPlace = findBookPlace(student->next);
			else {
				bookPlace = malloc(sizeof(takenBook));
				student->next = bookPlace; }
			readerPlace->student = student;
			readerPlace->date = date;
			readerPlace->next = NULL;

			bookPlace->book = book;
			bookPlace->date = date;
			bookPlace->next = NULL;
		}
	}
	fclose(takenCount);
	fclose(takenRead);
}

static void freeTakenBooks(takenBook *book) {
	if (book == NULL) return;
	if (book->next != NULL) freeTakenBooks(book->next);
	free(book->next);
}

static void freeReaders(takenByStudent *student) {
	if (student == NULL) return;
	if (student->next != NULL) freeReaders(student->next);
	free(student->date);
	free(student->next);
}

void freeStudentData(studentData *student) {
	free(student->ID);
	free(student->surname);
	free(student->name);
	free(student->fathername);
	free(student->faculty);
	free(student->specialty);
	freeTakenBooks(student->next);
	free(student->next);
}

void freeBookData(bookData *book) {
	free(book->ISBN);
	free(book->title);
	free(book->author);
	freeReaders(book->next);
	free(book->next);
}

void freeAllStudents(node *nodePointer) {
	if (nodePointer != NULL) {
		freeAllStudents(nodePointer->left);
		freeStudentData(nodePointer->studentDataPnt);
		free(nodePointer->studentDataPnt);
		freeAllStudents(nodePointer->right);
		free(nodePointer); }
}

void freeAllBooks(node *nodePointer) {
	if (nodePointer != NULL) {
		freeAllBooks(nodePointer->left);
		freeBookData(nodePointer->bookDataPnt);
		free(nodePointer->bookDataPnt);
		freeAllBooks(nodePointer->right);
		free(nodePointer); }
}

int checkBook(char *ISBN) {
	bookData *book = findBook(booksRoot, ISBN);
	if (book->taken == 0) {
		int days=0;
		char *closestDate = NULL;
		takenByStudent *next = book->next;
		while (next != NULL) {
			if (dateToDays(next->date) > days) {
				days = dateToDays(next->date);
				closestDate = next->date; }
			next = next->next;
		}
		printf("Доступных к выдаче книг нет. ");
		if (closestDate != NULL)
		printf("Ближайшая книга будет сдана %s.\n", closestDate);
		else printf("Данные о датах возврата книги отсутствуют в базе.\n");
		return 0;
	}
	return 1;
}

void giveBook(char *ID, char *ISBN) {
	bookData *book = findBook(booksRoot, ISBN);
	studentData *student = findStudent(studentsRoot, ID);
	takenBook *bookPlace;
	takenByStudent *readerPlace;
	if (book!=NULL && student!=NULL) {
		takenByStudent *next = book->next;
		while (1) {
			if (next == NULL) break;
			if (strcmp(next->student->ID, ID) == 0) {
				printTime();
				fprintf(
					logfile,
					"%s tried to give the second book with ISBN %s to student with ID %s\n", user, ISBN, ID);
				printf("Эта книга на руках у этого студента уже есть.\n");
				return; }
			next = next->next;
		}
		if (book->next != NULL) readerPlace = findReaderPlace(book->next);
		else {
			readerPlace = malloc(sizeof(takenByStudent));
			book->next = readerPlace; }
		if (student->next != NULL) bookPlace = findBookPlace(student->next);
		else {
			bookPlace = malloc(sizeof(takenBook));
			student->next = bookPlace; }
		char *date = NULL, day[2], month[2], year[4];
		while (1) {
			startCycle:
			free(date);
			date=readData(12);
			int c=0, i=0;
			if (date != NULL) {
				for (i=0; i<10; i++) {
					if (i==2 || i==5) {if (isdigit(*(date+i)) != 0) goto startCycle;}
					else if (isdigit(*(date+i)) == 0) goto startCycle;
					if (i<=1) day[i] = *(date+i);
					else if (i>=3 && i<=4) month[i-3] = *(date+i);
					else if (i>=6 && i<=9) year[i-6] = *(date+i);
				}
				if (atoi(day) > 31) goto startCycle;
				if (atoi(month) > 12) goto startCycle;
				if (atoi(year)>2050 || atoi(year)<2000) goto startCycle;
				goto x;
			}
		}
	x:	readerPlace->student = student;
		readerPlace->date = date;
		readerPlace->next = NULL;
		book->taken--;
		bookPlace->book = book;
		bookPlace->date = date;
		bookPlace->next = NULL;
		printTime();
		fprintf(
			logfile,
			"%s gave book with ISBN %s to student with ID %s\n", user, ISBN, ID);
		printf("Книга успешно выдана студенту.\n");
	}
	else {
		printf("Студент не найден.\n");
		return; }
}

void returnBook(char *ID, char *ISBN) {
	bookData *book = findBook(booksRoot, ISBN);
	studentData *student = findStudent(studentsRoot, ID);
	if (student == NULL) {
		printf("Студент не найден.\n");
		printTime();
		fprintf(logfile, "%s tried to took book with ISBN %s from unexistend student\n", user, ISBN);
		return;	}
	else {
		takenByStudent *reader = book->next;
		takenByStudent *prevReader = NULL;
		if (reader == NULL) {printf("У студента нет этой книги.\n"); return;}
		while (strcmp(reader->student->ID, ID) != 0) {
			if (reader == NULL) {report(8); return;}
			prevReader = reader;
			reader = reader->next;
		}
		book->taken++;
		free(reader->date);
		if (prevReader == NULL) book->next = reader->next;
		else prevReader->next = reader->next;
		free(reader);

		takenBook *takenBookPnt = student->next;
		takenBook *prevBook = NULL;
		while (strcmp(takenBookPnt->book->ISBN, ISBN) != 0) {
			prevBook = takenBookPnt;
			takenBookPnt = takenBookPnt->next;
		}
		if (prevBook == NULL) student->next = takenBookPnt->next;
		else prevBook->next = takenBookPnt->next;
		free(takenBookPnt);

	}
	printf("Книга успешно принята у студента.\n");
	printTime();
	fprintf(logfile, "%s took book with ISBN %s from student with ID %s\n", user, ISBN, ID);

}
