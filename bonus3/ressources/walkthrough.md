



# bonus3


## description
- Il y a un exécutable **bonus3**.
- sans arguments, il n'affiche rien
- avec un argument(a par exemple), il affiche un retour à la ligne.

## Résolution

      0x08048502 <+14>:    mov    edx,0x80486f0
       0x08048507 <+19>:    mov    eax,0x80486f2
       0x0804850c <+24>:    mov    DWORD PTR [esp+0x4],edx
       0x08048510 <+28>:    mov    DWORD PTR [esp],eax
       0x08048513 <+31>:    call   0x8048410 <fopen@plt>
On voit que le programme récupère le flag et le stock dans une variable.


Ensuite on voit qu'il y a une comparaison entre un buffer et argv[1].
Si on veut recuperer le flag, on doit resoudre cette condition.
La condition attends que le buffer soit une chaine vide. 

       0x080485e3 <+239>:   mov    DWORD PTR [esp+0x8],0x0
       0x080485eb <+247>:   mov    DWORD PTR [esp+0x4],0x8048707
       0x080485f3 <+255>:   mov    DWORD PTR [esp],0x804870a
       0x080485fa <+262>:   call   0x8048420 <execl@plt>
On voit qu'apres la comparaison, il y a un appel a **execl** avec en argument "/bin/sh"

donc il suffit de transmettre en arguments une chaine vide pour résoudre ca problème et avoir accès a un **shell**

    cat /home/user/end/.pass
    3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
