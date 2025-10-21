#ifndef VECTOR_H
#define VECTOR_H

struct vector_s {
  int            taille  ;      /* taille du tableau content */
  unsigned char* content ;      /* tableau d'éléments de @$\llbracket 0, 255 \rrbracket$@ */
};
typedef struct vector_s * vector;


/*déclarations des fonctions concernant l'objet vector*/
vector create_zero_vector (int n);
void delete_vector(vector v);
void print_vector(vector v);
double distance(vector v1, vector v2);

#endif
