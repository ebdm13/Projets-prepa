#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "jeu_quadrant.h"
#include "database.h"
#include "vector.h"

int quadrant(vector v){
    assert(v->taille == 2);
    int x = v->content[0];
    int y = v->content[1];

    // On code la classe sur deux bits: b1b0
    int b0 = (x > 127);
    int b1 = (y > 127);
    return 2 * b1 + b0;

}

database fabrique_jeu_donnees(int db_size){
    database db = create_empty_database(db_size);
    for (int i = 0; i < db_size; i++) {
        int x = rand() % 256;
        int y = rand() % 256;

        db->datas[i].vector = create_zero_vector(2);
        db->datas[i].vector->content[0] = x;
        db->datas[i].vector->content[1] = y;
        db->datas[i].class = quadrant(db->datas[i].vector);
    }
    return db;
}
