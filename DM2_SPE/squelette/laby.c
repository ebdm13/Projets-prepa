#include "laby.h"
#include "uf.h"
#include <stdlib.h>

void draw_laby(laby_t laby) {
	//affiche le labyrinthe laby avec des caractères ASCII
  for (int j = 0 ; j < laby.width ; j ++) {
    printf("+-");
  }
  printf("+\n");
  for (int i = 0 ; i < laby.height-1 ; i ++) {
    printf("| ");
    for (int j = 0 ; j < laby.width-1 ; j ++) {
      int c = laby.cells[i*laby.width+j];
      if (c % 2 == 1) {printf("| ");}
      else {printf("  ");}
    }
    printf("|\n");
    printf("+");
    for (int j = 0 ; j < laby.width ; j ++) {
      int c = laby.cells[i*laby.width+j];
      if ((c/2) % 2 == 1) {printf("-+");}
      else {printf(" +");}
    }
    printf("\n");
  }
  printf("| ");
  for (int j = 0 ; j < laby.width-1 ; j ++) {
    int c = laby.cells[(laby.height-1)*laby.width+j];
    if (c % 2 == 1) {printf("| ");}
    else {printf("  ");}
  }
  printf("|\n");
  printf("+");
  for (int j = 0 ; j < laby.width ; j ++) {
    printf("-+");
  }
  printf("\n");
}





void draw_laby_with_visited(laby_t laby, bool* visited) {
	//affiche le labyrinthe laby et le chemin décrit par visited en ASCII
  for (int j = 0 ; j < laby.width ; j ++) {
    printf("+-");
  }
  printf("+\n");
  for (int i = 0 ; i < laby.height-1 ; i ++) {
    printf("|");
    for (int j = 0 ; j < laby.width-1 ; j ++) {
      if (visited[i*laby.width+j]) {printf("·");}
      else {printf(" ");}
      int c = laby.cells[i*laby.width+j];
      if (c % 2 == 1) {printf("|");}
      else {
        if (visited[i*laby.width+j] && visited[i*laby.width+j+1]) {printf("·");}
        else {printf(" ");}
      }
    }
    if (visited[i*laby.width+laby.width-1]) {printf("·");}
    else {printf(" ");}
    printf("|\n");
    printf("+");
    for (int j = 0 ; j < laby.width ; j ++) {
      int c = laby.cells[i*laby.width+j];
      if ((c/2) % 2 == 1) {printf("-+");}
      else {
        if (visited[i*laby.width+j] && visited[(i+1)*laby.width+j]) {printf("·+");}
        else {printf(" +");}
      }
    }
    printf("\n");
  }
  printf("|");
  for (int j = 0 ; j < laby.width-1 ; j ++) {
    if (visited[(laby.height-1)*laby.width+j]) {printf("·");}
    else {printf(" ");}
    int c = laby.cells[(laby.height-1)*laby.width+j];
    if (c % 2 == 1) {printf("|");}
    else {
      if (visited[(laby.height-1)*laby.width+j] && visited[(laby.height-1)*laby.width+j+1]) {printf("·");}
      else {printf(" ");}
    }
  }
  if (visited[(laby.height-1)*laby.width+laby.width-1]) {printf("·");}
  printf("|\n");
  printf("+");
  for (int j = 0 ; j < laby.width ; j ++) {
    printf("-+");
  }
  printf("\n");
}


bool is_laby_plein(laby_t laby) {
	//teste si le labyrinthe laby est plein
  for(int k=0; k < laby.height; k++){
  	if (laby.cells[k] != 3) return false;
  	//3 est le type des cases ac mur sud et mur est
  }
  return true;
}


int linearise(laby_t laby, int i, int j) {
	//indice ds le tableau unidimensionnel de la case de coordonnées (i,j)
	//dans le tableau bidimensionnel aux dimensions de laby
	return i * laby.width + j ;
}

void delinearise(laby_t laby, int x, int* pi, int* pj) {
	//modifie les valeurs de *pi et *pj pour qu'ils enregistrent les
	//coordoonées dans le tableau bidimensionnel aux dimensions de laby *
	//de la case d'indice x dans le tableau  unidimensionnel correspondant
  *pi = x / laby.width;
  *pj = x % laby.width;
}


bool is_in_laby(laby_t laby, int i, int j) {
  return i >= 0 && j >= 0 && i < laby.height && j < laby.width;
  // teste si la case de coordonnées (i,j) est dans laby
}

bool can_go_from(laby_t laby, int i1, int j1, int i2, int j2) {
	//hypothèse : is_in_laby(laby,i1,j1) && is_in_laby(laby,i2,j2)
	//teste l'abscence de mur entre les cases (i1,j1) et (i2,j2) ds laby
  if      (i1 == i2     && j2 == j1 + 1 ) //1 à gauche, 2 à droite
  	{return laby.cells[linearise (laby, i1, j1 )] % 2 == 0;     }
  else if (i1 == i2     && j2 == j1 - 1 ) //2 à gauche, 1 à droite
  	{return laby.cells[linearise (laby, i2, j2 )] % 2 == 0;     }
  else if (i2 == i1 + 1 && j1 == j2     ) //1 au dessus, 2 en dessous
  	{return laby.cells[linearise (laby, i1, j1 )] / 2 % 2 == 0; }
  else if (i1 == i2 + 1 && j1 == j2     ) //2 au dessus, 1 en dessous
  	{return laby.cells[linearise (laby, i2, j2 )] / 2 % 2 == 0; }
  else {return false;}
}



void casse_mur(laby_t laby, int i1, int j1, int i2, int j2) {
	//hypothèse : is_in_laby(laby,i1,j1) && is_in_laby(laby,i2,j2)
	//casse le mur entre la case (i1,j1) et la case (i2,j2) dans laby
  if      (i1 == i2   && j2 == j1 + 1 )
  	{laby.cells[linearise (laby, i1, j1 )] = (laby.cells[linearise (laby, i1, j1 )] / 2 ) * 2;}
  else if (i1 == i2   && j2 == j1 - 1 )
  	{laby.cells[linearise (laby, i2, j2 )] = (laby.cells[linearise (laby, i2, j2 )] / 2 ) * 2;}
  else if (i2 == i1+1 && j1 == j2     )
  	{laby.cells[linearise (laby, i1, j1 )] =  laby.cells[linearise (laby, i1, j1 )] % 2;}
  else if (i1 == i2+1 && j1 == j2     )
  	{laby.cells[linearise (laby, i2, j2 )] =  laby.cells[linearise (laby, i2, j2 )] % 2;}
}

laby_t gen_laby_full(int w, int h){
    laby_t laby;
    laby.height = h;
    laby.width = w;
    laby.cells = malloc(sizeof(char) * h * w);
    for (int i = 0; i < h * w; i++) {
        laby.cells[i] = 3;
    }
    return laby;
}

void rec_generator(laby_t laby, bool* visited, int i, int j){
    int cases[4][2] = {{i+1, j}, {i-1, j}, {i, j+1}, {i, j-1}};
    for (int k = 0; k<4; k++) {
        int r = rand() % (4 - k);
        int new_i = cases[r+k][0], new_j = cases[r+k][1];
        cases[r+k][0] = cases[k][0], cases[r+k][1] = cases[k][1];
        cases[k][0] = new_i, cases[k][1] = new_j;

        int cl = linearise(laby, new_i, new_j);
        if (is_in_laby(laby, new_i, new_j) && !visited[cl]){
            visited[cl] = true;
            casse_mur(laby, i, j, new_i, new_j);
            rec_generator(laby, visited, new_i, new_j);
        }
    }
}

void generate_laby(laby_t laby){
    bool* visited = calloc(laby.height * laby.width, sizeof(bool));
    visited[0] = true;
    rec_generator(laby, visited, 0, 0);
    free(visited);
}

mur_t* tab_murs_laby_plein(laby_t laby){
    int w = laby.width, h = laby.height;
    mur_t* murs = malloc(sizeof(mur_t) * (2*w*h - w - h));
    int k = 0;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++){
            if (j < w-1){
                murs[k].i1 = i; murs[k].j1 = j; murs[k].i2 = i; murs[k++].j2 = j+1;
            }
            if (i < h - 1){
                murs[k].i1 = i; murs[k].j1 = j; murs[k].i2 = i+1; murs[k++].j2 = j;
            }
        }
    }
    return murs;
}

void melange_liste_murs(mur_t* murs, int n){
    for (int i = n-1; i > 0 ; i--) {
        int j = rand() % (i+1);
        mur_t temp = murs[i];
        murs[i] = murs[j];
        murs[j] = temp;
    }
}

void generate_laby2(laby_t laby){
    int w = laby.width, h = laby.height;
    int n = 2*w*h - w - h;
    mur_t* murs= tab_murs_laby_plein(laby);
    melange_liste_murs(murs, n);
    uf_partition_t p = uf_initialize(w*h);
    int breaked_wall = 0;
    int i_mur = 0;
    while (breaked_wall < w*h-1){
        assert(i_mur < n);
        int c1 = linearise(laby, murs[i_mur].i1, murs[i_mur].j1);
        int c2 = linearise(laby, murs[i_mur].i2, murs[i_mur].j2);
        if (uf_find(p[c1]) != uf_find(p[c2])){
            casse_mur(laby, murs[i_mur].i1, murs[i_mur].j1, murs[i_mur].i2, murs[i_mur].j2);
            uf_union(p[c1], p[c2]);
            breaked_wall++;
        }
        i_mur++;
    }
    free(murs);
    uf_free(p, w*h);
}

bool rec_solver(laby_t laby, bool* chemin, int i, int j){
    int cases[4][2] = {{i+1, j}, {i-1, j}, {i, j+1}, {i, j-1}};
    bool contain_s = (i == laby.height - 1 && j == laby.width - 1);
    for (int k = 0; k < 4; k++) {
        int i2 = cases[k][0], j2 = cases[k][1];
        int cl = linearise(laby, i2, j2);
        if (is_in_laby(laby, i2, j2) && !chemin[cl] && can_go_from(laby, i, j, i2, j2)){
            chemin[cl] = true;
            bool contain_s2 = rec_solver(laby, chemin, i2, j2);
            contain_s = contain_s || contain_s2;
            chemin[cl] = contain_s2;
        }
    }
    return contain_s;
}

bool* solve_labyrinthe(laby_t laby){
    bool* chemin = calloc(laby.width * laby.height, sizeof(bool));
    chemin[0] = true;
    rec_solver(laby, chemin, 0, 0);
    return chemin;
}

void build_wall(laby_t laby, int i1, int j1, int i2, int j2){
    if (i1 <= i2 && j1 <= j2){
        laby.cells[linearise(laby, i1, j1)] += (j2 - j1) + 2 * (i2 - i1);
    } else {
        laby.cells[linearise(laby, i2, j2)] += (j1 - j2) + 2 * (i1 - i2);
    }
}

void build_walls(laby_t laby, bool* visited, int i, int j, int parent){
    int cases[4][2] = {{i+1, j}, {i-1, j}, {i, j+1}, {i, j-1}};
    for (int k = 0; k < 4; k++) {
        int i2 = cases[k][0], j2 = cases[k][1];
        int cl = linearise(laby, i2, j2);
        if (is_in_laby(laby, i2, j2) && can_go_from(laby, i, j, i2, j2) && cl != parent && visited[cl]){
            build_wall(laby, i, j, i2, j2);
        } else if (is_in_laby(laby, i2, j2) && can_go_from(laby, i, j, i2, j2) && !visited[cl]){
            visited[cl] = true;
            build_walls(laby, visited, i2, j2, linearise(laby, i, j));
        }
    }
}

void repare(laby_t laby){
    int w = laby.width, h = laby.height;
    bool* visited = calloc(w * h, sizeof(bool));
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int cl = linearise(laby, i, j);
            if (!visited[cl]){
                visited[cl] = true;
                build_walls(laby, visited, i, j, cl);
                if (j > 0){
                    casse_mur(laby, i, j, i, j-1);
                } else if (i > 0) {
                    casse_mur(laby, i, j, i-1, j);
                }
            }
        }
    }
    free(visited);
}
