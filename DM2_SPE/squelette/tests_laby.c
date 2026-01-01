#include "laby.h"
#include <stdlib.h>

int main() {
    srand(time(NULL));
    laby_t laby = gen_laby_full(10, 5);
    int w = laby.width, h = laby.height;
    assert(is_laby_plein(laby));
    generate_laby2(laby);
    // mur_t* murs = tab_murs_laby_plein(laby);
    // int size = 2*w*h - w - h;
    // for (int k = 0; k < size; k++) {
    //     casse_mur(laby, murs[k].i1, murs[k].j1, murs[k].i2, murs[k].j2);
    // }
    draw_laby(laby);
    free(laby.cells);
    // free(murs);
}
