#include <stdio.h>
#include <stdlib.h>

int m = 0; //0x804988c

int v(void)
{
	char buffer[512];

	fgets(buffer, 512, stdin);
	printf(buffer);
	if (m == 64)
	{
		fwrite("Wait what?!\n", 12, 1, stdout);
		system("/bin/sh");
	}
	return (0);
}

int main(void)
{
	v(); //0x08048520 <+6>:     call   0x80484a4 <v>
	return (0);

	0x804988c