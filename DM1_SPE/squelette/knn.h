#ifndef KNN_H
#define KNN_H

#include "database.h"
#include "candidats.h"

extern int nb_class_max;

/* retourne le tableau des indices dans db->datas des k plus proches vecteurs de input */
candidats pproche_list(database db, int k, vector input);

/* retourne la classe la plus présente parmi les données classifiées dans db repérées
par leur indice dans lc. */
int classe_majoritaire(database db, candidats lc);

/* retoure la classe la plus présente parmi les k plus proches voisins
de input dans db*/
int classify_list(database db, int k, vector input);

#endif
