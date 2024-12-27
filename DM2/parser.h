#pragma once
#include "AST.h"	

typedef TokenNode* (*add_command_t)(TokenNode* head, AST_node* root);

/*Déplace la tête après la commande*/
TokenNode* ignore_command(TokenNode* head);

/*Ignore la commande ou l'ajoute si c'est un appelle de fonction*/
 TokenNode* add_unknown(TokenNode* head, AST_node* root);

/*Vérifie que head correspond à la définition d'une variable*/
bool is_var(TokenNode* head);

/*Ajoute la variable dans l"AST*/
TokenNode* add_var(TokenNode* head, AST_node* root);

/*Renvoie la racine de l'AST construit à partir de la queu de la liste de token*/
AST_node* parse(TokenNode* TokenQueue);