#include "token.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

void free_token(Token* token){
	assert(token != NULL);
	free(token->lexem);
	free(token);
}

Token* create_token(TokenType type, char* lexem){
	assert(lexem != NULL);
	Token* token = malloc(sizeof(Token));
	token->type = type;
	token->lexem = strdup(lexem);
	return token;
}

TokenNode* add_token(TokenNode *head, Token* token, int linenumber){
	assert(head != NULL);
	assert(token != NULL);
	TokenNode* new_node = malloc(sizeof(TokenNode));	
	new_node->token = token;
	new_node->linenumber = linenumber;
	new_node->next = NULL;
	while (head->next != NULL){
		head = head->next;
	}
	head->next = new_node;
	return new_node;
}

TokenNode* insert_token(TokenNode* head, Token* token, int linenumber){
	assert(head != NULL);
	assert(token != NULL);
	TokenNode* new_node = malloc(sizeof(TokenNode));	
	new_node->token = token;
	new_node->linenumber = linenumber;
	new_node->next = head->next;
	head->next = new_node;
	return new_node;
}

void free_list(TokenNode* queue){
	assert(queue != NULL);
	TokenNode* current = queue;
	TokenNode* next;
	while (current->next != NULL){
		next = current->next;
		free_token(current->token);
		free(current);
		current = next;
	}
	free_token(current->token);
	free(current);
}

char* type_name_map[] = {
	"COMMAND_UNKNOWN",
	"COMMAND_VERSION",
	"COMMAND_HEADER",
	"COMMAND_RELATIVE",
	"COMMAND_CLEF",
	"COMMAND_KEY",
	"COMMAND_MAJOR",
	"COMMAND_MINOR",
	"COMMAND_TIME",
	"COMMAND_REPEAT",
	// Dynamiques / Nuances
	"COMMAND_PPP",
	"COMMAND_PP",
	"COMMAND_P",
	"COMMAND_MP",
	"COMMAND_MF",
	"COMMAND_F",
	"COMMAND_FF",
	"COMMAND_FFF",
	"COMMAND_FP",
	"COMMAND_SF",
	"COMMAND_SFP",
	"COMMAND_SFZ",
	"COMMAND_RFZ",
	// Points d'Articulation
	"COMMAND_ACCENT",
	"COMMAND_TENUTO",
	"COMMAND_STACCATO",
	"COMMAND_MARCATO",
	// Ornements
	// \>
	"COMMAND_R_ANGLE",
	// \<
	"COMMAND_L_ANGLE",
	// \!
	"COMMAND_EX_POINT",
	"COMMAND_MORDENT",
	"COMMAND_APPOGGIATURA",	
	"COMMAND_PRALL",
	"COMMAND_FERMATA",
	"COMMAND_TURN",
	"COMMAND_TRILL",
	// Autres
	"COMMAND_BAR",
	"COMMAND_PARTIAL",
	"VAR_NAME",
	"VAR_CALL",
	"REST",
	"NOTE",
	// a b c d e f g
	"NOTE_NAME",
	// 1 2 4 8 16 32 64
	"DUREE",
	// is es ih eh
	"ACCIDENTAL",
	"OCTAVE",
	// ~
	"TIE",
	// .
	"DOT",
	// |
	"BARLINE",
	// <
	"LEFT_ANGLE",
	// >
	"RIGHT_ANGLE",
	"IDENTIFIER",
	// =
	"EQUAL",
	// Ex: 7
	"NUMBER",
	// )
	"RIGHT_PAREN",
	// (
	"LEFT_PAREN",
	// {
	"LEFT_BRACE",
	// }
	"RIGHT_BRACE",
	// "truc"
	"STRING",
	// #
	"HASH",
	"BACKSLACH",
	// Begininf of Tokens
	"BOT",
	// End Of Tokens
	"EOT",
};

char* get_type(Token* token){
	return type_name_map[token->type];
}

bool is_type_command(Token *token){
	// Méthode archaïque, mais permet de ne pas modifier la fonction à chaque 
	//fois qu'on ajoute ou enlève une commande.
	bool ok = true;
	char* type = get_type(token);
	if (strlen(type) > 8) {
		for (int i = 0; i < 8;i++){
			switch (i) {
				case 0: ok = type[0] == 'C'; break;
				case 1: ok = type[1] == 'O'; break;
				case 2: ok = type[2] == 'M'; break;
				case 3: ok = type[3] == 'M'; break;
				case 4: ok = type[4] == 'A'; break;
				case 5: ok = type[5] == 'N'; break;
				case 6: ok = type[6] == 'D'; break;
				case 7: ok = type[7] == '_'; break;
			}
			if (!ok){
				return false;
			}
		}
		return true;
	}
	return false;
}

void print_token(Token* token){
	assert(token != NULL);
	printf("(%s): %s\n", type_name_map[token->type], token->lexem);	
}

void print_tokens(TokenNode *queue){
	assert(queue != NULL);
	TokenNode* node = queue;
	while (node->next != NULL){
		printf("l:%d ", node->linenumber);
		print_token(node->token);
		node = node->next;
	}
	print_token(node->token);
}

void test_token(){
	TokenNode* queu = malloc(sizeof(TokenNode));
	queu->token = create_token(BOT, "");
	queu->next = NULL;
	TokenNode* head = add_token(queu, create_token(NUMBER, "45"), 0);
	head = add_token(head, create_token(EQUAL, "="), 0);
	head = add_token(head, create_token(NUMBER, "45"), 0);
	head = add_token(head, create_token(EOT, ""), 0);
	print_tokens(queu);
	free_list(queu);
}
