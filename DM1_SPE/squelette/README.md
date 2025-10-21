Les fichiers de ce squelette sont là pour vous faciliter l'organisation 
du code du DM1 (exercice 1) en fichiers `.h`, `.c` et tests, ainsi que
leur compilation (nécessairement compilation séparée).

Avant de commencer prennez connaissance des fichiers `.h`.
Ils indiquent dans quel fichier doit être codée quelle fonction, et
vous évitent de recopier manuellement les définitions de types données dans l'énoncé.

Vous pouvez vous laisser guider par l'énoncé pour l'ordre dans lequel 
les fonctions doivent être écrites, mais veillez à les écrire dans les
bons fichiers.

Les fichiers `tests_section_...` sont là pour acceuillir vos jeux de tests
de la section correspondante. En général il est conseillé de tester chaque
fonction juste après sa définition, avant de passer à la suite.
Celà dit, tester un classifieur sans base de données est difficile...
C'est pourquoi on n'attend pas de tests à la fin de la section 4, mais
à la fin de la section 5 qui a pour objet la création d'un petit jeu de
données.

Un petit rappel concernant la compilation séparée des fichiers est donné
dans le dossier "Outils/fiches_outils" sur cahier de prépa.

Pour l'utilisation de ce Makefile, c'est assez simple :
	- `make XXX.o` compile le fichier `XXX.c` sans édition de lien, 
	ça produit donc un fichier `XXX.o` non exécutable ;
	- `make test_section_i` compile le fichier `test_section_i.c` avec
	édition de liens,	ça produit donc un exécutable `test_section_i` ;
	- `make all` lance toutes les commande `make test_section_i` ; 
	- `make clean` supprime tous les fichiers potentiellement crées par
	des commandes de compilations de ce Makefile, les `.o` et les exécutables.
	
Pour des explications plus poussées sur les Makefile, vous pouvez 
retrouver les explications données dans le DM1 des MP2I en 2021/2022 sur
[http://perso.eleves.ens-rennes.fr/~afalq494/docs_mp2i/DM/DM1.pdf](http://perso.eleves.ens-rennes.fr/~afalq494/docs_mp2i/DM/DM1.pdf)
