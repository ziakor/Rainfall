# level0


## description
- Il y a un exécutable **level0**.Lorsqu'on l'exécute, il y a une *Segmentation Fault*. Si on lui donne un argument(par exemple 42) il affiche **"No  !"**

## Résolution

En utilisant gdb, on peut savoir par mal de chose.

    gdb level0
    
### Sans arguments
    Starting program: /home/user/level0/level0
    
    Program received signal SIGSEGV, Segmentation fault.
    0x08049aff in ____strtol_l_internal ()
Avec la commande **backtrace**, on peut savoir tout ce qui s'est passé avant le crash.

    0x08049aff in ____strtol_l_internal ()
    (gdb) backtrace
    #0  0x08049aff in ____strtol_l_internal ()
    #1  0x08049aaa in strtol ()
    #2  0x0804972f in atoi ()
    #3  0x08048ed9 in main ()

On découvre que le crash provient du **atoi**.
### Avec arguments

On ajoute un argument au programme dans gdb.

    (gdb) set arg 42
Ca ne crash plus.
En utilisant la commande disas, on va pouvoir voir toute ce que le programme a fait.

    (gdb) disas main

Résultat:

    Dump of assembler code for function main:
    [...]
       0x08048ed4 <+20>:    call   0x8049710 <atoi>
       0x08048ed9 <+25>:    cmp    $0x1a7,%eax
	[...]
    End of assembler dump.

On peut voir qu'il y a l'appel d'**atoi** puis une comparaison avec une valeur, **0x1a7** ce qui correspond a **423** en décimal.

Quand on test de lancer le programme avec en argument 423, il y a un shell qui s'ouvre.
On peut voir aussi dans l'assembleur

       0x08048f4a <+138>:   movl   $0x80c5348,(%esp)
       0x08048f51 <+145>:   call   0x8054640 <execv>
Quand on affiche la valeur de **0x80c5348**

    (gdb) x/s 0x80c5348
    0x80c5348:       "/bin/sh"

Si la comparaison du **atoi** est vrai alors **execve** lance /bin/sh

    $ whoami
    level1

Reste a afficher le contenu du **.pass**.

    $ cat /home/user/level1/.pass
    1fe8a524fa4bec01ca4ea2a869af2a02260d4a7d5fe7e7c24d8617e6dca12d3a


