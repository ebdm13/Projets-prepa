#pragma once
#include "AST.h"	

typedef TokenNode* (*add_command_t)(TokenNode* head, AST_node* node, TokenNode* TokenQueue,AST_node* root);

/*Concatène s1 et s2 et renvoie la chaine de charactère créée*/
char* concat(char* s1, char* s2);

/*En cas d'erreur, cette fonction est appelé est libére la mémoire alouée pour TokenQueue et root puis quit le programme*/
void clean_exit(TokenNode* TokenQueue, AST_node* root);

/*Vérifie que head correspond à la définition d'une variable*/
bool is_var(TokenNode* head, TokenNode* TokenQueue, AST_node* root);

/*Ajoute un élément (commande ou autre) au parent du contexte: node*/
TokenNode* add_context_element(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue);

/*Ajoute la variable head commen enfant de node dans l'AST*/
TokenNode* add_var(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue);

/*Vérifie si name est un appelle correcte à une variable*/
bool is_var_defined(AST_node* parent, char* name);

/*Essaye d'ignorer la commande en renvoyant le noeud de token le plus proche qui est une autre commande ou une définition de variable.*/
TokenNode* ignore_command(TokenNode* head, TokenNode *TokenQueue, AST_node *root);

/*Ignore la commande ou l'ajoute si c'est un appelle de variable*/
 TokenNode* add_unknown(TokenNode* head, AST_node* node, TokenNode *TokenQueue, AST_node* root);

/*Renvoie la racine de l'AST construit à partir de la queu de la liste de token*/
AST_node* parse(TokenNode* TokenQueue);