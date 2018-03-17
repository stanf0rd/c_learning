//программа позволяет выбрать, из какой системы счисления переводить в десятичную
//работает с числами, в десятичном виде умещающимися в long long unsigned int
//указатели, функции, динамическое выделение памяти, мануальчик и прочие радости прилагаются

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long long int bicycle(int exponent, int num)
{
	unsigned long long int c=1;
	int i;
	for (i=1; i<=exponent; i++) c=c*num;
	return c;
}

unsigned long long int conv_to_dec(int symbAmount, int numSyst, int *pntHex)
{
	unsigned long long int dec=0;
	int i;
	for (i=1; i<=symbAmount; i++) dec=(dec+bicycle(symbAmount-i, numSyst)*(*pntHex++));
	return dec;
}

void man(int key)
{
	if (key==0)
	{
		printf("********************************\n*        Welcome to the        *\n*   numeric system converter   *\n");
		printf("*            manual!           *\n*     ********************     *\n*   Convertation to dec from   *\n");
		printf("*  2-16 based numeric systems  *\n*         is supported.        *\n*     ********************     *\n");
		printf("*    Use keys from -2 to -16   *\n*   to choose numeric system.  *\n*     ********************     *\n");
		printf("*    To see the manual again   *\n* run the program without keys.*\n********************************\n");
	}
	if (key==2) printf("*Error: too much digits in the number.\n");
	if (key==3) printf("*Error: invalid keys.\n*Run program without keys to read manual.\n");
	if (key==4) printf("*Error: invalid digits.\n");
	exit(key);
}

int char_to_int(char charCode, int numSyst)
{
	int num=0;
	if (('0'<=charCode)&&(charCode<='9')) num=charCode-'0'; //'0' -> int 0...
	else if (('A'<=charCode)&&(charCode<='H')) num=charCode-'A'+11;
	else if (('a'<=charCode)&&(charCode<='f')) num=charCode-'a'+11;
	else man(4);
	if (num>=numSyst) man(4);
	return num;
}

void startMessg(int numSyst, int digits)
{
	printf("Program started in %d-to-10 mode.\n", numSyst);
	printf("Type numbers in base-%d numeral system, separated by a space.\n", numSyst);
	printf("Maximal length of each number is %d digits.\nBase %d: ", digits, numSyst);
}

void main(int argc, char *argv[])
{
	unsigned long long int dec=0;
	int numSyst, readDigits=0, *pntHex, check, wasMinus=0;
	int allowedDigits[]={0,0,64,40,32,27,24,22,21,20,19,18,17,17,16,16,16};
	//massive: idea by Artem Andrukhov, thanks!
	char c=0;
	//checking if arguments are right, else open manual or error
	if (argc==1) man(0);
	if ( (argc!=2) || (argv[1][0]!='-') || (strlen(argv[1])>3) || (strlen(argv[1])<2) ) man(3);
	//counting and checking numeric system
	if (strlen(argv[1])==2) numSyst = char_to_int(argv[1][1], 10);
	else numSyst = 10*char_to_int(argv[1][1], 10) + char_to_int(argv[1][2], 10);
	//reading maximal number of digits from the massive
	if ( (numSyst<2) || (numSyst>16) ) man(2);
	startMessg(numSyst, allowedDigits[numSyst]);
	//allocating memory and reading number
	pntHex = malloc(allowedDigits[numSyst]*sizeof(int));
	while((c=getchar())!=EOF)
	{
		while(c==' ') {c=getchar();}
		while((readDigits<=allowedDigits[numSyst])&&(c!=' ')&&(c!='\n'))
		{
			if (c=='-') {wasMinus=1; c=getchar();}
			*(pntHex+readDigits)=char_to_int(c, numSyst);
			readDigits++;
			c=getchar();
		}
		//checking amount of symbols, converting and printing
		if (readDigits>allowedDigits[numSyst]) man(2); //excess symbol found, exit
		else
		{
			dec=conv_to_dec(readDigits, numSyst, pntHex);
			if (check==0) {printf("Base 10: "); check=1;}
			if(wasMinus==1) {wasMinus=0; printf("-");}
			if (dec!=0) printf("%llu ", dec);
		}
		readDigits=0;
		if (c=='\n') {printf("\nBase %d: ", numSyst); check=0;}
	}
	free(pntHex);
	printf("\n");
}