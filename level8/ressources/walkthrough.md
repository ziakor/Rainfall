# level8


## description
- Il y a un exécutable **level8**.
- L'executable a une boucle infini qui affiche (nil), (nil)

## Résolution
En desassemblant le main avec gdb, on voit qu'il n'y a pas de fonction cachée,  tout est dans le main.

       0x08048575 <+17>:    mov    0x8049ab0,%ecx
       0x0804857b <+23>:    mov    0x8049aac,%edx
       0x08048581 <+29>:    mov    $0x8048810,%eax
       0x08048586 <+34>:    mov    %ecx,0x8(%esp)
       0x0804858a <+38>:    mov    %edx,0x4(%esp)
       0x0804858e <+42>:    mov    %eax,(%esp)
       0x08048591 <+45>:    call   0x8048410 <printf@plt>

Il y a la déclaration de plusieurs variable global

**0x8049ab0** = service
**0x8049aac** = auth
Ensuite il y a **printf** avec en argument "**%p, %p\n**", **auth**, **service**.
Qui initialement sont a **NULL**.
le tout dans une boucle infinie.
.

Il y a plusieurs choix de commande: auth, reset, service et login.

Par exemple:

       0x080486ee <+394>:   movl   $0x8048833,(%esp)
       0x080486f5 <+401>:   call   0x8048480 <system@plt>
Il y a un appel **system** a "**/bin/sh**"

       0x080486e2 <+382>:   mov    eax,ds:0x8049aac
       0x080486e7 <+387>:   mov    eax,DWORD PTR [eax+0x20]
       0x080486ea <+390>:   test   eax,eax
**test** est équivalent a un **cmp**. ici on va comparer **auth**[32] a **0** et auth[32] doit etre different.
Si la comparaison est correcte alors ca appel **system**

Pour pouvoir accéder a **system**() il faut remplir certaines conditions.


Donc notre objectif va être  que **auth**[32] soit différent de 0 quand on appel **login** pour que le shell se lance.

A chaque tour de boucle on a l'adresse de auth et de service

    0x804a008, (nil) 
Donc notre objectif ca va etre l'adresse 0x804a028 ne soit pas vide.
On appel une première fois **auth** avec un argument(**auth a** par exemple)
Pour cela on va appeler deux fois **service** pour que le **strdup** remplisse l'adresse  0x804a028.

On finit par utiliser login

    cat /home/user/level9/.pass
    c542e581c5ba5162a85f767996e3247ed619ef6c6f7b76a59435545dc6259f8a
