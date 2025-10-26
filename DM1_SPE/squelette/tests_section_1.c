#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#include "database.h"
#include "vector.h"

int main() {
    // test vecteur vide
    vector vide = create_zero_vector(0);
    print_vector(vide);
    printf("\n");
    delete_vector(vide);

    // test vecteurs
    int n = 5;
    vector v1 = create_zero_vector(n);
    print_vector(v1);
    printf("\n");
    vector v2 = create_zero_vector(n);
    v2->content[0] = 1;
    v2->content[1] = 2;
    v2->content[2] = 3;
    v2->content[3] = 4;
    v2->content[4] = 5;
    print_vector(v2);
    printf("\n");
    double d = distance(v1, v2);
    printf("distance: %f \n", d);
    assert(d == sqrt(55));
    delete_vector(v1);
    delete_vector(v2);

    // test database
    database db = create_empty_database(2);
    db->datas[0].class = 1;
    db->datas[0].vector = create_zero_vector(3);
    db->datas[0].vector->taille = 3;
    db->datas[0].vector->content[0] = 0;
    db->datas[0].vector->content[1] = 1;
    db->datas[0].vector->content[2] = 2;

    db->datas[1].class = 0;
    db->datas[1].vector = create_zero_vector(3);
    db->datas[1].vector->taille = 3;
    db->datas[1].vector->content[0] = 2;
    db->datas[1].vector->content[1] = 5;
    db->datas[1].vector->content[2] = 4;

    print_database(db);
    delete_database(db);
	return 0;
}
