#include "laby.h"
#include <stdlib.h>

laby_t gen_random_laby(int w, int h){
   laby_t laby = gen_laby_full(w, h);
   mur_t* murs = tab_murs_laby_plein(laby);
   int n = 2*w*h - w - h;
   melange_liste_murs(murs, n);
   int d = rand() % n;
   for (int k = 0; k < d; k++) {
       casse_mur(laby, murs[k].i1, murs[k].j1, murs[k].i2, murs[k].j2);
   }
   free(murs);
   return laby;
}

int main() {
    srand(time(NULL));
    // laby_t laby = gen_laby_full(10, 5);
    // int w = laby.width, h = laby.height;
    // assert(is_laby_plein(laby));
    // generate_laby(laby);
    // mur_t* murs = tab_murs_laby_plein(laby);
    // int size = 2*w*h - w - h;
    // for (int k = 0; k < size; k++) {
    //     casse_mur(laby, murs[k].i1, murs[k].j1, murs[k].i2, murs[k].j2);
    // }
    // bool* chemain = solve_labyrinthe(laby);
    // draw_laby_with_visited(laby, chemain);
    // draw_laby(laby);
    // free(laby.cells);
    // free(chemain);
    // free(murs);
    //
    laby_t laby = gen_random_laby(10, 5);
    draw_laby(laby);
    repare(laby);
    draw_laby(laby);
    free(laby.cells);
}
