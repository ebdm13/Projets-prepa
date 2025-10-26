#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "knn.h"
#include "candidats.h"

int nb_class_max;
//penser à lui donner une valeur dans les fonctions main selon l'application

candidats pproche_list(database db, int k, vector input){
    assert(db->size >= k);
    candidats l = NULL;
    int r = 0;
    for (int i = 0; i < db->size; i++) {
        r = insertion_list(&l, r, k, db, i, input);
    }
    return l;
}

int classe_majoritaire(database db, candidats lc){
    int nb_elem_class[nb_class_max];
    for (int i = 0; i < nb_class_max; i++){
        nb_elem_class[i] = 0;
    }
    int max = 0;
    int class_max = 0;
    for (candidats next = lc; next != NULL; next = next->next){
        int c = db->datas[next->indice].class;
        nb_elem_class[c] += 1;
        if (nb_elem_class[c] > max){
            max = nb_elem_class[c];
            class_max = c;
        }
    }
    return class_max;
}

int classify_list(database db, int k, vector input){
    assert(db->size >= k);
    candidats lc = pproche_list(db, k, input);
    int res = classe_majoritaire(db, lc);
    delete_liste(lc);
    return res;
}
