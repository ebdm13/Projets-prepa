#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>


#include "database.h"
#include "knn.h"
#include "lecture_mnist.h"

/* Retourne le nombre de charactères dans n.
 * Ex: 123 -> 3*/
int nb_chars (int n){
    int res = 1;
    n /= 10;
    while (n != 0){
        res += 1;
        n /= 10;
    }
    return res;
}

/* Renvoie la matrice de confusion construite avec le jeu de données d'entraînement: db_train
   le jeu de données de test: db_test en appliquant l'algorithme des k plus proches voisins
*/
int** gen_matrice_confusion(database db_train, database db_test, int k){
    int** mat_conf = calloc(10, sizeof(int*));
    for (int i = 0; i < 10; i++) {
        mat_conf[i] = calloc(10, sizeof(int));
    }

    for(int i = 0; i < db_test->size; i++){
        int prediction = classify_list(db_train, k, db_test->datas[i].vector);
        mat_conf[db_test->datas[i].class][prediction] += 1;
    }
    return mat_conf;
}

/* Libère la mémoire allouée pour mat_conf */
void delete_matrice_confusion(int** mat_conf){
    for (int i = 0; i < 10; i++) {
        free(mat_conf[i]);
    }
    free(mat_conf);
}

/* Affiche n espaces */
void print_spaces(int n){
    for (int i = 0; i < n; i++){
        printf(" ");
    }
}

/* Affiche la matrice de confusion */
void print_matrice_confusion(int** mat_conf, int nb_max_chars){
    printf("   ");
    for (int j = 0; j < 10; j++){
        printf("\e[1m   %d\e[m", j);
    }
    printf("\n");

    for (int i = 0; i < 10; i++){
        printf("\e[1m %d \e[m", i);
        if (i == 0){
            printf("\e[1m⎡\e[m");
        } else if (i == 9){
            printf("\e[1m⎣\e[m");
        } else {
            printf("\e[1m⎢\e[m");
        }
        for (int j = 0; j < 10; j++){
            int nb_spaces = nb_max_chars - nb_chars(mat_conf[i][j]) + (j != 0);
            print_spaces(nb_spaces);
            if (i == j){
                printf("\e[1;32m%d\e[m", mat_conf[i][j]);
            } else if (mat_conf[i][j] != 0){
                printf("\e[31m%d\e[m", mat_conf[i][j]);
            } else {
                printf("%d", mat_conf[i][j]);
            }
            print_spaces(j == 9);
        }

        if (i == 0){
            printf("\e[1m⎤\e[m\n");
        } else if (i == 9){
            printf("\e[1m⎦\e[m\n");
        } else {
            printf("\e[1m⎥\e[m\n");
        }
    }
}

int  main(){
	nb_class_max = 10;
	database* pdb_train = malloc(sizeof(database));
	database* pdb_test = malloc(sizeof(database));

	clock_t start = clock();
	mnist_input(10000, pdb_train, 1000, pdb_test);
	clock_t end1 = clock();
	int** mat_conf = gen_matrice_confusion(*pdb_train, *pdb_test, 3);
	clock_t end2 = clock();

	printf("\n");
	print_spaces(3 * 4);
	printf("\e[1m Matrice de confusion \e[m\n\n");
	print_matrice_confusion(mat_conf, 3);

	double delta1 = (double)(end1 - start) / CLOCKS_PER_SEC;
	double delta2 = (double)(end2 - end1) / CLOCKS_PER_SEC;
	printf("\n\nTemps de lecture du fichier: %.3fs\n", delta1);
	printf("Temps de génération de la matrice: %.3fs\n", delta2);

	delete_matrice_confusion(mat_conf);
	delete_database(*pdb_train);
	delete_database(*pdb_test);
	free(pdb_train);
	free(pdb_test);
	return 0;
}
