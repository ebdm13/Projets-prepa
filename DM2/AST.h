#pragma once

#include "token.h"
#include <stdbool.h>

typedef struct AST_node {
	Token* token;
	int numberline;
	struct AST_node* parent;
	struct AST_node* first_child;	
	struct AST_node* next_sibling;
} AST_node;

/*Renvoie un pointeur vers le nouveaux noeud de l'AST*/
AST_node* create_AST_node(Token* token, int numberline);

/*Ajoute child commen enfant de parent*/
void add_child(AST_node* parent, AST_node* child);

/*Libère la mémoire alouée pour l'AST SAUF POUR TOKEN !!*/
void free_AST(AST_node* root);

/*Affiche un noeud de l'AST avec une indentation initiale de indent tabulations 
où sticks indique si il y a un baton ou non en fonction de l'indentation du noeud. Par défaut, sticks doit-être initialisé à true.
*/
void print_AST_node(AST_node* root, int indent, bool sticks[100]);

/*Affiche l'AST*/
void print_AST(AST_node* root);