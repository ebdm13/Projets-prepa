# Rapport
## Satsolver
**Q1:** Le premier argument est l'appelle du programme: ./satsolver.
Sans deuxième argument: Fatal error: exception Invalid_argument("index out of bounds")

**Q10::** Soit n le nombre de noeud de l'arbre syntaxique de f. (les noueuds sont les éléments de bases: Not, Or, And, Var, Top, Bot).
Ainsi, on se ramène à un problème de parcours d'arbre binaires avec une étape de traitement (union) en $\Theta(n)$.
Or on sait que le pire cas pour un parcours en hauteur est le peigne.
De plus, dans le pire cas toutes les feuilles sont des variables différentes. Et le nombre de feuilles est forcément inférieure aux nombres
de noeuds et dans le pire cas: peigne, il y a $E(\frac {n+1}2) = \Theta(n)$ feuilles. 
Pour un tel arbre, l'équation de complexitée s'écrit: $C(n) = C(n-1) + \Theta(n)$.
Donc $C(n) = \Theta(n)$. Pour optimiser, on peut implémenter var_list avec un ARN ou un AVL.
