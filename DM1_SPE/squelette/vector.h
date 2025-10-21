#ifndef VECTOR_H
#define VECTOR_H

struct vector_s {
  int            taille  ;      /* taille du tableau content */
  unsigned char* content ;      /* tableau d'éléments de @$\llbracket 0, 255 \rrbracket$@ */
};
typedef struct vector_s * vector;


/*déclarations des fonctions concernant l'objet vector*/

// retourne un vecteur de taille n nouvellement alloué, dont toutes les coordonnées sont initialisées à 0.
vector create_zero_vector (int n);

// libère l’espace mémoire occupé par v
void delete_vector(vector v);

// affiche v
void print_vector(vector v);

// calcule la distance euclidienne entre deux vecteurs de même taille
double distance(vector v1, vector v2);

#endif
