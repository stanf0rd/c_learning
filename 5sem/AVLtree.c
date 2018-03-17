//сбалансированное АВЛ-дерево

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lineIndex=1;
struct node;

struct node* insert(struct node *, char *);
unsigned char height(struct node *);
struct node *balance(struct node *);
struct node *rotateLeft(struct node *);
struct node *rotateRight(struct node *);
void treeprint(struct node *);
char *scanFile(FILE *, FILE *, int *);
void error(int);
void man();

struct node {
	char *name;
	int count;
	int line;
	unsigned char height;
	struct node *left;
	struct node *right;
};

void main(int argc, char *argv[]) {
	if (argc==1) man();
	if (argc!=2) error(1);
	FILE *fileCount = fopen(argv[1], "r"), *fileRead = fopen(argv[1], "r");
	if (fileCount == NULL) error(2);
	struct node *root=NULL, *nodePointer;
	char *wordPnt;
	while ((wordPnt = scanFile(fileCount, fileRead, &lineIndex)) != NULL) 
		root = insert(root, wordPnt);
	treeprint(root);
	exit(0);
}

struct node* insert(struct node* p, char *wordPnt) {
	if (p == NULL) {
		struct node *newNodePointer = (struct node *)malloc(sizeof(struct node));
		newNodePointer->name = wordPnt;
		newNodePointer->count = 1;
		newNodePointer->line = lineIndex;
		p = newNodePointer; }
	else {
		int diff = strcmp(wordPnt, p->name);
		if (diff==0) {
			p->count++;
			p->line = lineIndex; }
		else if (diff < 0) p->left = insert(p->left, wordPnt);
		else p->right = insert(p->right, wordPnt);
	}
	return balance(p);
}

unsigned char height(struct node *p) {
	return p?p->height:0;
}

int bfactor(struct node *p) {
	return height(p->right)-height(p->left);
}

void fixheight(struct node *p) {
	unsigned char heightLeft = height(p->left);
	unsigned char heightRight = height(p->right);
	p->height = (heightLeft>heightRight ? heightLeft : heightRight)+1;
}

struct node *rotateLeft(struct node *q) { //левый поворот вокруг q
	struct node *p = q->right;
	q->right = p->left;
	p->left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

struct node *rotateRight(struct node *p) { //правый поворот вокруг p
	struct node *q = p->left;
	p->left = q->right;
	q->right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

struct node *balance(struct node *p) { //балансировка узла p
	fixheight(p);
	if (bfactor(p) == 2) {
		if (bfactor(p->right) < 0) p->right = rotateRight(p->right);
		return rotateLeft(p); }
	if (bfactor(p) == -2) {
		if (bfactor(p->left) > 0) p->left = rotateLeft(p->left);
		return rotateRight(p); }
	return p; // балансировка не нужна
}

void treeprint(struct node *nodePointer) {
	if (nodePointer != NULL) {
		treeprint(nodePointer->left);
		printf("%-20s число вхождений: %-6d", nodePointer->name, nodePointer->count);
		printf("последнее на стр. %-10d строке %3d \n", nodePointer->line/50, nodePointer->line%50);
		treeprint(nodePointer->right); }
}

char* scanFile(FILE *fileCount, FILE *fileRead, int *lineIndex) {
	int i=0, c=0;
	while(c = fgetc(fileCount)) {
		if (c=='\n') (*lineIndex)++;
		if ((c<'0' || c>'z')||(c>'9' && c<'A')||(c>'Z' && c<'a')) { 
			if (i==0) {if (fgetc(fileRead) == EOF) return NULL;}
			else break; } 
		else i++; }
	char *word = malloc(i*sizeof(char));
	for (int j=0; j<=i; j++) *(word+j)=fgetc(fileRead);
	*(word+i)='\0';
	return word;
}

void man() {
	printf(" ********************************\n *    Use only one argument:    *\n");
	printf(" *       ./a.out file.txt       *\n *   ************************   *\n");
	printf(" *    To see the manual again   *\n * run the program without keys.*\n");
	printf(" ********************************\n");
	exit(0);
}

void error(int key) {
	if (key==1) printf("Error. Invalid arguments.\n");
	if (key==2) printf("Error. Invalid file path, file name or no permission.\n");
	if (key==3) printf("Error. File is empty.\n");
	exit(key); 
}