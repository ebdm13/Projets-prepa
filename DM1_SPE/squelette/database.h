#ifndef DATABASE_H
#define DATABASE_H

#include "vector.h"

struct classified_data_s {
  vector vector ;                   /* le vecteur */
  int    class  ;                   /* sa classe */
};

struct database_s {
  int size ;                        /* taille du jeu de données */
  struct classified_data_s* datas ; /* tableau contenant les données classifiées */
};

typedef struct database_s * database;

/* alloue la mémoire pour une base de données de taille n, initialement vide,
 et la retourne. */
database create_empty_database (int n);

/* libère l’espace mémoire occupé par db */
void delete_database(database db);

/* affiche db */
void print_database(database db);

#endif
