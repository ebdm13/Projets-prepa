#include "uf.h"
#include <stdlib.h>

void print_uf_partition_verbose(uf_partition_t p, int n) {
  // hypothèse : p est une partition de [0..n[
  // affiche pour chaque élém de [0..n[ la remontée jusqu'à une racine
  // dans la structure union find p
  for (int i = 0; i < n; i ++) {
    uf_elem_t cursor = p[i];
    for (; cursor->parent != cursor; cursor = cursor->parent) {
      printf("%d ~> ", cursor->elem);
    };
    printf("%d", cursor->elem);
    printf("\n");
  }
}

// Renvoie une partition en singletons de [|0, size-1|]
uf_partition_t uf_initialize(int size){
    if (size > 0){
        uf_partition_t res = malloc(size * sizeof(uf_elem_t));
        for (int i = 0; i < size; i++) {
            res[i] = malloc(sizeof(struct uf_elem_s));
            res[i]->elem = i;
            res[i]->parent = res[i];
            res[i]->rank = 0;
        }
        return res;
    } else{
        return NULL;
    }
}

// Libère l'espace aloué pour p de taille size
void uf_free(uf_partition_t p, int size){
    for (int i = 0; i < size; i++) {
        free(p[i]);
    }
    free(p);
}

// Retourne le représentant de la classe de x.
uf_elem_t uf_find_no(uf_elem_t x){
    uf_elem_t current = x;
    while (current != current->parent){
        current = current->parent;
    }
    return current;
}

// Retourne le représentant de la classe de x en raccourcissant les chemins des noeuds croisés.
uf_elem_t uf_find(uf_elem_t x){
    if (x == x->parent){
        return x;
    } else {
        uf_elem_t p = uf_find(x->parent);
        x->parent = p;
        return p;
    }
}

// Réunie les classes de x et de y.
void uf_union(uf_elem_t x, uf_elem_t y){
    x = uf_find(x);
    y = uf_find(y);
    if (x->rank <= y->rank){
        x->parent = y;
        y->rank += (int)(x->rank == y->rank && x!=y);
    } else {
        y->parent = x;
    }
}
