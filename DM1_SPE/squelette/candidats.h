#ifndef CANDIDATS_H
#define CANDIDATS_H

#include "database.h"

typedef struct s_cellule* candidats;
struct s_cellule {
  int     indice ;              /* indice du point dans le jeu de donnée */
  double  distance ;            /* distance au point de recherche */
  candidats next;
};

/*alloue une cellule pour représenter le candidat d’indice ind et
 de distance dist et retourne la liste réduite à ce candidat */
candidats create_list(int ind, double dist);

/* libère l’espace mémoire occupé par toutes les cellules de la liste lc */
void delete_liste(candidats lc);

/* affiche les candidats d’une liste de type candidats */
void print_list_candidats(candidats l);

/* modifie la liste triée pointée par pl, qui contient initialement r candi-
dats, pour y faire apparaître le candidat db->datas[i] si sa distance à input le justifie. Et
retourne le nombre de candidats dans *pl à l’issue de l’insertion*/
int insertion_list(candidats* pl, int r, int k, database db, int i, vector input);


#endif
