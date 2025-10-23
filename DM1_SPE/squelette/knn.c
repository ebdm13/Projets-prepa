#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "knn.h"
#include "candidats.h"

int nb_class_max;
//penser à lui donner une valeur dans les fonctions main selon l'application

candidats pproche_list(database db, int k, vector input){
    assert(db->size >= k);
    candidats* l = malloc(sizeof(candidats));
    *l = NULL;
    int r = 0;
    for (int i = 0; i < db->size; i++) {
        r = insertion_list(l, r, k, db, i, input);
    }
    candidats res = *l;
    free(l);
    return res;
}

int classe_majoritaire(database db, candidats lc){
    int* nb_elem_class = calloc(nb_class_max, sizeof(int));
    int max = 0;
    int class_max = 0;
    candidats next = lc;
    int count = 0;
    while (next != NULL){
        count++;
        int class = db->datas[next->indice].class;
        nb_elem_class[class] += 1;
        if (nb_elem_class[class] > max){
            max = nb_elem_class[class];
            class_max = class;
        }
        next = next->next;
    }
    free(nb_elem_class);
    return class_max;
}

int classify_list(database db, int k, vector input){
    assert(db->size >= k);
    candidats lc = pproche_list(db, k, input);
    int res = classe_majoritaire(db, lc);
    delete_liste(lc);
    return res;
}
