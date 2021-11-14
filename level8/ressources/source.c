#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *auth = NULL;
char *service = NULL;

int main(void)
{
	char buffer[128];

	while (1)
	{
		//  0x08048581 <+29>:    mov    eax,0x8048810 //! "%p, %p \n"
		// 	   0x08048586 <+34>:    mov    DWORD PTR [esp+0x8],ecx
		//  0x0804858a <+38>:    mov    DWORD PTR [esp+0x4],edx
		//  0x0804858e <+42>:    mov    DWORD PTR [esp],eax
		//  0x08048591 <+45>:    call   0x8048410 <printf@plt>
		printf("%p, %p\n", auth, service);

		//  0x08048596 <+50>:    mov    eax,ds:0x8049a80
		//  0x0804859b <+55>:    mov    DWORD PTR [esp+0x8],eax
		//  0x0804859f <+59>:    mov    DWORD PTR [esp+0x4],0x80
		//  0x080485a7 <+67>:    lea    eax,[esp+0x20]
		//  0x080485ab <+71>:    mov    DWORD PTR [esp],eax
		//  0x080485ae <+74>:    call   0x8048440 <fgets@plt>
		//  0x080485b3 <+79>:    test   eax,eax
		if (fgets(buffer, 128, stdin) == 0)
			break;
		if (strncmp(buffer, "auth ", 5) == 0)
		{
			auth = malloc(4);
			auth[0] = 0;
			if (strlen(buffer + 5) <= 30)
				strcpy(auth, buffer + 5);
		}
		if (strncmp(buffer, "reset", 5) == 0)
			free(auth);
		if (strncmp(buffer, "service", 6) == 0)
			service = strdup(buffer + 7);
		if (strncmp(buffer, "login", 5) == 0)
		{
			if (auth[32] != 0)
				system("/bin/sh");
			else
				fwrite("Password:\n", 10, 1, stdout);
		}
	}
}