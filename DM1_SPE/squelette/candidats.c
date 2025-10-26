#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "candidats.h"

candidats create_list(int ind, double dist){
    candidats lc = malloc(sizeof(struct s_cellule));
    lc->indice = ind;
    lc->distance = dist;
    lc->next = NULL;
    return lc;
}

void delete_liste(candidats lc){
   candidats next = lc;
   while (next != NULL){
      candidats prev = next;
      next = prev->next;
      free(prev);
   }
}

void print_list_candidats(candidats l){
    printf("{\n");
    candidats next = l;
    while (next != NULL){
        printf("\t i: %d, d: %f\n", next->indice, next->distance);
        next = next->next;
    }
    printf("}\n");
}

int insertion_list(candidats* pl, int r, int k, database db, int i, vector input){
    assert(i < db->size && i >= 0);
    assert(r <= k);
    assert(pl != NULL);
    double d = distance(input, db->datas[i].vector);
    candidats next = *pl;
    candidats prev = NULL;

    // évite d'ajouter un élément puis de l'enlever inutilement
    if ((*pl) != NULL && d >= (*pl)->distance && r == k) {
        return r;
    }

    while (next != NULL && d < next->distance){
        prev = next;
        next = next->next;
    }

    candidats new = create_list(i, d);

    if (prev == NULL){
        *pl = new;
    } else {
        prev->next = new;
    }
    new->next = next;

    if (r == k){
        candidats to_delete = *pl;
        *pl = (*pl)->next;
        free(to_delete);
    } else {
        r++;
    }
    return r;
}
