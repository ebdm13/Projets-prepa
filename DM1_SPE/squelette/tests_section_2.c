#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


#include "database.h"
#include "candidats.h"
#include "vector.h"


int main() {
    // test de create_list et delete_liste
    candidats lc = create_list(2, 3.4567);
    assert(lc != NULL);
    assert(lc->indice == 2);
    assert(lc->distance == 3.4567);
    assert(lc->next == NULL);
    delete_liste(lc);

    // test de insertion_list et print_list_candidats
    database db = create_empty_database(3);
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

    db->datas[2].class = 2;
    db->datas[2].vector = create_zero_vector(3);
    db->datas[2].vector->taille = 3;
    db->datas[2].vector->content[0] = 1;
    db->datas[2].vector->content[1] = 2;
    db->datas[2].vector->content[2] = 3;

    printf("database: \n");
    print_database(db);
    candidats* l = malloc(sizeof(candidats));
    *l = NULL;
    int r = 0;
    vector input = create_zero_vector(3);

    printf("\n------------------------------\n");
    printf("candidats: \n");

    r = insertion_list(l, r, 3, db, 0, input);
    assert(r==1);
    assert((*l)->indice == 0);
    assert((*l)->distance == distance(input, db->datas[0].vector));
    assert((*l)->next == NULL);
    print_list_candidats(*l);

    r = insertion_list(l, r, 3, db, 1, input);
    assert(r==2);
    assert((*l)->indice == 1);
    assert((*l)->distance == distance(input, db->datas[1].vector));
    assert((*l)->next != NULL);

    assert((*l)->next->indice == 0);
    assert((*l)->next->distance == distance(input, db->datas[0].vector));
    assert((*l)->next->next == NULL);
    print_list_candidats(*l);

    r = insertion_list(l, r, 3, db, 2, input);
    assert(r==3);

    assert((*l)->indice == 1);
    assert((*l)->distance == distance(input, db->datas[1].vector));
    assert((*l)->next != NULL);

    assert((*l)->next->indice == 2);
    assert((*l)->next->distance == distance(input, db->datas[2].vector));
    assert((*l)->next->next != NULL);

    assert((*l)->next->next->indice == 0);
    assert((*l)->next->next->distance == distance(input, db->datas[0].vector));
    assert((*l)->next->next->next == NULL);
    print_list_candidats(*l);

    r = insertion_list(l, r, 3, db, 1, input);
    assert(r==3);
    assert((*l)->indice == 1);
    assert((*l)->distance == distance(input, db->datas[1].vector));
    assert((*l)->next != NULL);

    assert((*l)->next->indice == 2);
    assert((*l)->next->distance == distance(input, db->datas[2].vector));
    assert((*l)->next->next != NULL);

    assert((*l)->next->next->indice == 0);
    assert((*l)->next->next->distance == distance(input, db->datas[0].vector));
    assert((*l)->next->next->next == NULL);
    print_list_candidats(*l);

    r = insertion_list(l, r, 3, db, 0, input);
    assert(r==3);
    assert((*l)->indice == 2);
    assert((*l)->distance == distance(input, db->datas[2].vector));
    assert((*l)->next != NULL);

    assert((*l)->next->indice == 0);
    assert((*l)->next->distance == distance(input, db->datas[0].vector));
    assert((*l)->next->next != NULL);

    assert((*l)->next->next->indice == 0);
    assert((*l)->next->next->distance == distance(input, db->datas[0].vector));
    assert((*l)->next->next->next == NULL);
    print_list_candidats(*l);

    delete_vector(input);
    delete_liste(*l);
    free(l);
    delete_database(db);

	return 0;
}
