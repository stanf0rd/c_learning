#include <stdio.h>
#include <stdlib.h>

void error(int key)
{
	if (key==1) printf("No lists found.\n");
	if (key==2) printf("Invalid list\n");
	exit(key);
}

void main()
{
	int c=0, d, i;
	printf("Waiting for string...\n");
	while ((c!='-') || (d==0))
	{
		d=c;
		c=getchar();
		if (c=='\n') error(1);
	}
	if ((c=getchar())=='\n') error(1);
	if (
		( (c<'0') || (d<'0') || (c>'9') || (d>'9') ) &&
		( (c<'A') || (d<'A') || (c>'Z') || (d>'Z') ) &&
		( (c<'a') || (d<'a') || (c>'z') || (d>'z') )
	) error(2);
	if (d<c) for (i=d; i<=c; i++) putchar(i);
	else for (i=d; i>=c; i--) putchar(i);
	printf("\n");
}