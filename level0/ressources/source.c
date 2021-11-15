#define _GNU_SOURCE
#include <stddef.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	// 0x08048ed4 <+20>:    call   0x8049710 <atoi> //! Utilisation de atoi
	if (atoi(argv[1]) == 423) //  0x08048ed9 <+25>:    cmp    eax,0x1a7 //! comparaison entre argv[1] et 0x1a7(423)
	{
		char *exec_args[2];
		// 0x08048ee0 <+32>:    mov    DWORD PTR [esp],0x80c5348 //! 0x80c5348 == "/bin/sh"
		// 0x08048ee7 <+39>:    call   0x8050bf0 <strdup>
		exec_args[0] = strdup("/bin/sh");
		exec_args[1] = NULL;
		gid_t egid = getgid(); //0x08048ef8 <+56>:    call   0x8054680 <getegid>
		uid_t euid = getuid(); //0x08048f01 <+65>:    call   0x8054670 <geteuid>

		setresgid(egid, egid, egid);
		setresuid(euid, euid, euid);

		execv(exec_args[0], exec_args);
	}
	else
	{
		// 0x08048f58 <+152>:   mov    eax,ds:0x80ee170 //! fwrite ecris dans stderr 0x80ee170 = stderr
		// 0x08048f68 <+168>:   mov    DWORD PTR [esp+0x8],0x5 //! 5 en argument de __n
		// 0x08048f70 <+176>:   mov    DWORD PTR [esp+0x4],0x1 //! 1 en argument de __size
		// 0x08048f5f <+159>:   mov    eax,0x80c5350 //! 0x80c5350 =  "No !\n"
		fwrite("No !\n", 1, 5, stderr);
	}
}
