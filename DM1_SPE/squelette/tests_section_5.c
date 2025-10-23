#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>


#include "candidats.h"
#include "database.h"
#include "knn.h"
#include "jeu_quadrant.h"
#include "vector.h"

int main (){
    srand(time(NULL));

	nb_class_max = 4;

	// test de quadrant
	vector input = create_zero_vector(2);
	input->content[0] = 4;
	input->content[1] = 189;
	assert(quadrant(input) == 2);

	input->content[0] = 200;
	input->content[1] = 189;
	assert(quadrant(input) == 3);

	input->content[0] = 78;
	input->content[1] = 2;
	assert(quadrant(input) == 0);

	input->content[0] = 234;
	input->content[1] = 1;
	assert(quadrant(input) == 1);

	// test de knn.c
	database db1 = create_empty_database(3);
    db1->datas[0].class = 1;
    db1->datas[0].vector = create_zero_vector(3);
    db1->datas[0].vector->taille = 3;
    db1->datas[0].vector->content[0] = 0;
    db1->datas[0].vector->content[1] = 1;
    db1->datas[0].vector->content[2] = 2;

    db1->datas[1].class = 0;
    db1->datas[1].vector = create_zero_vector(3);
    db1->datas[1].vector->taille = 3;
    db1->datas[1].vector->content[0] = 2;
    db1->datas[1].vector->content[1] = 5;
    db1->datas[1].vector->content[2] = 4;

    db1->datas[2].class = 1;
    db1->datas[2].vector = create_zero_vector(3);
    db1->datas[2].vector->taille = 3;
    db1->datas[2].vector->content[0] = 1;
    db1->datas[2].vector->content[1] = 2;
    db1->datas[2].vector->content[2] = 3;

    vector v = create_zero_vector(3);
    candidats l = pproche_list(db1, 3, v);
    assert(l->indice == 1);
    assert(l->distance == distance(v, db1->datas[1].vector));
    assert(l->next != NULL);

    assert(l->next->indice == 2);
    assert(l->next->distance == distance(v, db1->datas[2].vector));
    assert(l->next->next != NULL);

    assert(l->next->next->indice == 0);
    assert(l->next->next->distance == distance(v, db1->datas[0].vector));
    assert(l->next->next->next == NULL);

    assert(classe_majoritaire(db1, l) == 1);

    printf("database: \n");
    print_database(db1);
    printf("\ncandidats: \n");
    print_list_candidats(l);
    delete_database(db1);
    delete_liste(l);
    delete_vector(v);


	database db = fabrique_jeu_donnees(1000);
	float success = 0;
	float fail = 0;

	for (int i = 0; i < 1000; i++) {
        input->content[0] = rand() % 256;
    	input->content[1] = rand() % 256;
        if (classify_list(db, 1, input) == quadrant(input)){
            success += 1;
        } else { fail += 1;}
	}

	printf("Le taux de classification est: %f%\n", 100 * success / (success + fail));

	delete_database(db);
	delete_vector(input);
    return 0;
}
