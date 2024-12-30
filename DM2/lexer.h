#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "token.h"

/*Vérifie si s est une commande*/
bool isCommand(char* s);

/*Vérifie si s est une pause*/
bool isRest(char* s);

/*Vérifie si s est un nombre*/
bool isNumber(char* s);

/*Lit le f jusqu'a atteindre la fin du commentaire et incrémente linenumber en fonction*/
int skip_comment(FILE* f, int* linenumber, TokenNode* queue);

/*Lit f jusqu'a trouver un " et renvoie le token de la string*/
Token* get_string(FILE* f, int linenumber, TokenNode* queue);

/*Retourne le type de command*/
TokenType get_command_type(char* command);

/*Retourne le token correspondant à command*/
Token* get_command(char* command);

/*Retourne le token correspondant à rest*/
Token* get_rest(char* rest);

/*Retourne le token correspondant au buffer*/
Token* get_token(char* buffer);

/*Retourne la queu de la liste de token générée*/
TokenNode* lexer(FILE* f);