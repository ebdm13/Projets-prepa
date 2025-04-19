# Rapport
## Satsolver
**Q1:** Le premier argument est l'appelle du programme: ./satsolver.
Sans deuxième argument: Fatal error: exception Invalid_argument("index out of bounds")

**Q10:** Soit n le nombre de noeud de l'arbre syntaxique de f. (les noueuds sont les éléments de bases: Not, Or, And, Var, Top, Bot).
Ainsi, on se ramène à un problème de parcours d'arbre binaires avec une étape de traitement (union) en $\Theta(n)$.
Or on sait que le pire cas pour un parcours en hauteur est le peigne.
De plus, dans le pire cas toutes les feuilles sont des variables différentes. Et le nombre de feuilles est forcément inférieure aux nombres
de noeuds et dans le pire cas: peigne, il y a $E(\frac {n+1}2) = \Theta(n)$ feuilles. 
Pour un tel arbre, l'équation de complexitée s'écrit: $C(n) = C(n-1) + \Theta(n)$.
Donc $C(n) = \Theta(n)$. Pour optimiser, on peut implémenter var_list avec un ARN ou un AVL.

**Q19:**  Une famille en $\Theta(n^2)$ est un peigne avec que des Or et où les feuilles sont des 
variables sauf la dernière qui est un Top. i.e "X1 | X2 | ... | Xn | T". Car simpl_step vas parcourir
tout l'arbre pour faire qu'une seul simplification à chaque fois.

Q25 et 26 à faire

**Q31:** La formule générée par au_plus_une est sous FNC si et seulement si les formules en entrée sont des littéraux. 
De plus, si on considère que la taille de la formule généré comme la taille de son arbre syntaxique où les feuilles sont
les formules atomique en entré, alors sa taille est: 3n(n-1) - 1. Avec n le nombre de formules en entrée.