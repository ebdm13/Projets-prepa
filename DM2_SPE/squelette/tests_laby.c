#include "laby.h"
#include <stdlib.h>

/* Génère un labyrinth quelconque aléatoirement */
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

laby_t ex_laby1(){
    laby_t laby;
    laby.width = 3;
    laby.height = 3;
    laby.cells = calloc(9, sizeof(char));
    laby.cells[0] = 1;
    laby.cells[1] = 1;
    laby.cells[3] = 1;
    laby.cells[7] = 1;
    laby.cells[5] = 2;
    return laby;
}

laby_t ex_laby2(){
    laby_t laby;
    laby.width = 3;
    laby.height = 3;
    laby.cells = calloc(9, sizeof(char));
    laby.cells[1] = 2;
    laby.cells[3] = 1;
    laby.cells[4] = 2;
    return laby;
}

laby_t ex_laby3(){
    laby_t laby;
    laby.width = 3;
    laby.height = 3;
    laby.cells = calloc(9, sizeof(char));
    laby.cells[3] = 1;
    laby.cells[4] = 3;
    laby.cells[7] = 1;
    return laby;
}

/* Teste si la graphe sous-jacent du labyrinth formé des cases accessible depuis (i, j) dans laby
 * contient un cylcle, i.e si il existe deux cases accésible depuis (i,j) tel qu'il
 * existe plusieurs chemins possible les reliants.
 * visited est le tableau qui représente les cases déjà visité.
 * parent est la linéarisation de la case du prédécesseur dans le parcours,
 * où de (i, j) si il n'y en a pas.
*/
bool contient_cycle(laby_t laby, bool* visited, int i, int j, int parent){
    int cases[4][2] = {{i+1, j}, {i-1, j}, {i, j+1}, {i, j-1}};
    bool res = false;
    for (int k = 0; !res && k < 4; k++) {
        int i2 = cases[k][0], j2 = cases[k][1];
        int cl = linearise(laby, i2, j2);
        if (is_in_laby(laby, i2, j2) && can_go_from(laby, i, j, i2, j2) && cl != parent && visited[cl]){
            res = true;
        } else if (is_in_laby(laby, i2, j2) && can_go_from(laby, i, j, i2, j2) && !visited[cl]){
            visited[cl] = true;
            res = contient_cycle(laby, visited, i2, j2, linearise(laby, i, j));
        }
    }
    return res;
}

/* Teste si laby est un labyrinth parfait */
bool est_parfait(laby_t laby){
    int w = laby.width, h = laby.height;
    bool* visited = calloc(w*h, sizeof(bool));
    visited[0] = true;
    bool res = !contient_cycle(laby, visited, 0, 0, 0);
    int k = 0;
    while (res && k < w*h) {
        res = visited[k++];

    }
    free(visited);
    return res;
}

void tests_est_parfait(){
    laby_t laby1 = ex_laby1();
    laby_t laby2 = ex_laby2();
    laby_t laby3 = ex_laby3();

    assert(!est_parfait(laby1));
    assert(!est_parfait(laby2));
    assert(est_parfait(laby3));

    free(laby1.cells);
    free(laby2.cells);
    free(laby3.cells);
}

void tests_generate_laby(int n){
    for (int k = 0; k < n; k++) {
        laby_t laby = gen_laby_full(1 + rand() % 100, 1 + rand() % 100);
        generate_laby(laby);
        assert(est_parfait(laby));
        free(laby.cells);
    }
}

void tests_generate_laby_non_rec(int n){
    for (int k = 0; k < n; k++) {
        laby_t laby = gen_laby_full(1 + rand() % 100, 1 + rand() % 100);
        generate_laby_non_rec(laby);
        assert(est_parfait(laby));
        free(laby.cells);
    }
}

void tests_generate_laby2(int n){
    for (int k = 0; k < n; k++) {
        laby_t laby = gen_laby_full(1 + rand() % 100, 1 + rand() % 100);
        generate_laby2(laby);
        assert(est_parfait(laby));
        free(laby.cells);
    }
}

void tests_solver(){
    laby_t laby2 = ex_laby2();
    laby_t laby3 = ex_laby3();
    bool chemin1[9] = {1, 1 ,1, 0, 0, 1, 0, 0, 1};
    bool chemin2[9] = {1, 0 ,0, 1, 0, 0, 1, 1, 1};

    bool* solution1 = solve_labyrinthe(laby2);
    bool* solution2 = solve_labyrinthe(laby3);

    bool sol_chemin1 = true;
    bool sol_chemin2 = true;

    for (int k = 0; k < 9; k++) {
        assert(solution2[k] == chemin1[k]);
        sol_chemin1 = sol_chemin1 && (solution1[k] == chemin1[k]);
        sol_chemin2 = sol_chemin2 && (solution1[k] == chemin2[k]);
    }

    assert(sol_chemin1 || sol_chemin2);

    free(solution1);
    free(solution2);
    free(laby2.cells);
    free(laby3.cells);
}

void tests_repare(int n){
    laby_t laby1 = ex_laby1();
    laby_t laby2 = ex_laby2();
    laby_t laby3 = ex_laby3();

    repare(laby1);
    repare(laby2);
    repare(laby3);

    assert(est_parfait(laby1));
    assert(est_parfait(laby2));
    assert(est_parfait(laby3));

    laby_t laby1_original = ex_laby1();
    laby_t laby2_original = ex_laby2();
    laby_t laby3_original = ex_laby3();

    bool chemin1[9] = {1, 1 ,1, 0, 0, 1, 0, 0, 1};
    bool chemin2[9] = {1, 0 ,0, 1, 0, 0, 1, 1, 1};
    bool* sol = solve_labyrinthe(laby2);

    bool sol_chemin1 = true;
    bool sol_chemin2 = true;


    for (int k = 0; k < 9; k++) {
        assert(laby3.cells[k] == laby3_original.cells[k]);
        if (k != 5 && k != 7){
            assert(laby1.cells[k] == laby1_original.cells[k]);
        }

        sol_chemin1 = sol_chemin1 && (sol[k] == chemin1[k]);
        sol_chemin2 = sol_chemin2 && (sol[k] == chemin2[k]);
    }

    assert(laby1.cells[5] == 0 || laby1.cells[7] == 0);
    assert(sol_chemin1 || sol_chemin2);

    free(sol);
    free(laby1.cells);
    free(laby2.cells);
    free(laby3.cells);
    free(laby1_original.cells);
    free(laby2_original.cells);
    free(laby3_original.cells);

    for (int k = 0; k < n; k++) {
        laby_t laby = gen_random_laby(1 + rand() % 100, 1 + rand() % 100);
        repare(laby);
        assert(est_parfait(laby));
        free(laby.cells);
    }
}

void test_draw(){
    laby_t laby = gen_laby_full(40, 10);
	generate_laby2(laby);
    bool* solution = solve_labyrinthe(laby);
	
	draw_laby_with_visited(laby, solution);
    
	free(laby.cells);
	free(solution);
}

int main() {
    srand(time(NULL));
    tests_est_parfait();
    tests_generate_laby(100);
    tests_generate_laby_non_rec(100);
    tests_generate_laby2(100);
    tests_solver();
    tests_repare(100);
	test_draw();
}
