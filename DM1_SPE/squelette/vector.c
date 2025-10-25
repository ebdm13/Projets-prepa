#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h> // Attention à compiler avec l'option -lm

#include "vector.h"

vector create_zero_vector (int n){
    assert(n >= 0);
    vector v = malloc(sizeof(struct vector_s));
    v->taille = n;
    v->content = calloc(n, sizeof(unsigned char));
    return v;
}

void delete_vector(vector v){
    assert(v != NULL);
    free(v->content);
    free(v);
}

void print_vector(vector v){
    int n = v->taille;
    assert(n >= 0);
    printf("(");
    for (int i = 0; i < n - 1; i++){
        printf("%d, ", v->content[i]);
    }
    if (n - 1 >= 0){
        printf("%d", v->content[n-1]);
    }
    printf(")");
}

double distance(vector v1, vector v2){
    assert(v1->taille == v2->taille);
    int n = v1->taille;
    assert(n >= 0);
    int dist_square = 0;
    for (int i = 0; i < n; i++) {
        int diff = (v1->content[i] - v2->content[i]);
        dist_square += diff * diff;
    }
    //return sqrt(dist_square);
    return dist_square;
}
