#ifndef JEU_QUADRANT_H
#define JEU_QUADRANT_H


#include "vector.h"
#include "database.h"

/* retourne la classification de v en quadrant */
int quadrant(vector v);

/* retourne un jeu de db_size données aléatoire */
database fabrique_jeu_donnees(int db_size) ;

#endif
