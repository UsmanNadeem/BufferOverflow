#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
	char buf[100];
	int x;
	if(argc != 2)
	exit(1);
	x = 1;
	snprintf(buf, sizeof buf, argv[1]);
	buf[sizeof buf - 1] = 0;
	printf("buffer (%d): %s\n", strlen(buf), buf);
	printf("x is %d/%#x (@ %p)\n", x, x, &x);
	return 0;
}