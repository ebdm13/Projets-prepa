#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "database.h"
#include "vector.h"

database create_empty_database (int n){
    assert(n>=0);
    database db = malloc(sizeof(struct database_s));
    db->size = n;
    db->datas = malloc(n * sizeof(struct classified_data_s));
    return db;
}

void delete_database(database db){
    int n = db->size;
    assert(n>=0);
    for (int i = 0; i<n; i++) {
        delete_vector(db->datas[i].vector);
    }
    free(db->datas);
    free(db);
}

void print_database(database db){
    int n = db->size;
    assert(n>=0);
    printf("{\n");
    for (int i = 0; i < n ; i++) {
        printf("\t");
        print_vector(db->datas[i].vector);
        printf(" ~> %d\n", db->datas[i].class);
    }
    printf("}\n");

}
