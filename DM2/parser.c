#include "parser.h"
#include "lexer.h"	
#include "AST.h"
#include "error.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

char* concat(char* s1, char* s2){
	assert(s1 != NULL && s2 != NULL);
	char* s = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(s, s1);
	strcat(s, s2);
	return s;
}

void clean_exit(TokenNode *TokenQueue, AST_node *root){
	free_list(TokenQueue);
	free_AST(root);	
	exit(EXIT_FAILURE);
}

bool is_var(TokenNode* head, TokenNode* TokenQueue, AST_node* root){
	assert(head != NULL);
	assert(TokenQueue != NULL);
	assert(root != NULL);
	if (head->next != NULL && head->next->next != NULL
		&& head->token->lexem != NULL
		&& head->token->type == IDENTIFIER 
		&& head->next->token->type == EQUAL) {
		char* lexem = head->token->lexem;
		char* var_call = concat("\\", lexem);
		if (get_command_type(var_call) != COMMAND_UNKNOWN){
			error(head->linenumber, "Nom de variable réservé: %s", lexem);
			clean_exit(TokenQueue, root);
		}
		free(var_call);
		if (!isalpha(lexem[0]) && lexem[0] != '_'){
			error(head->linenumber, "Nom de variable non autorisé. Il doit commencer avec une lettre ou un _: %s", lexem);
			clean_exit(TokenQueue, root);
		}
		for (int i = 0; lexem[i] != '\0'; i++){
			if (!isalnum(lexem[i]) && lexem[i] != '_') {
				error(head->linenumber, "Nom de variable non autorisé. Il ne peut contenir que des chiffres, des lettres et _: %s", lexem);
				clean_exit(TokenQueue, root);
			}
		}
		return true;
	} else {
		return false;
	}
}

TokenNode* add_var(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue){
	assert(head != NULL);
	assert(root != NULL);
	head->token->type = VAR_NAME;
	AST_node* var_node = create_node(head);
	add_child(node, var_node);
	head = head->next->next;
	if (head->token->type == EOT) {
		error(head->linenumber, "Vous n'avez pas défini de valeur pour cette variable: %s", var_node->token->lexem);
		clean_exit(TokenQueue, root);
	}
	add_child(var_node, create_node(head));
	return head->next;
}

bool is_var_defined(AST_node *parent, char* name){
	while (parent->parent != NULL){
		parent = parent->parent;
		for (AST_node* child = parent->first_child; child != NULL; child=child->next_sibling){
			if (child->token->type == VAR_NAME) {
				char* var_call = concat("\\", child->token->lexem);
				if (strcmp(var_call, name) == 0){
					free(var_call);
					return true;
				}
				free(var_call);
			}
		}
	}
	return false;
}

TokenNode* ignore_command(TokenNode* head, TokenNode *TokenQueue, AST_node *root){
	assert(head != NULL);	
	TokenNode* node = head->next;
	int open_brace_cont = 0;
	while (node->token->type != EOT && ((!is_var(node, TokenQueue, root) && !is_type_command(node->token)) || open_brace_cont > 0)){
		if (node->token->type == LEFT_BRACE) {
			open_brace_cont++;
		} else if (node->token->type == RIGHT_BRACE) {
			open_brace_cont--;
		}
		node = node->next;
	}
	if (open_brace_cont < 0){
		error(node->linenumber, "'}' manquante");
		clean_exit(TokenQueue, root);
	}

	return node;
}

TokenNode* add_unknown(TokenNode* head, TokenNode *TokenQueue, AST_node* root){
	if (is_var_defined(root, head->token->lexem)){
		add_child(root, create_node(head));
		return head->next;
	}
	warning(head->linenumber, "Cette commande n'existe pas, fait référence à une commande non-définie ou n'a pas encore été implémentée: %s", head->token->lexem);
	return ignore_command(head, TokenQueue, root);
}

// liste des fonction pour ajouter une command à l'AST
add_command_t add_command_map[] = {add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown};

AST_node* parse(TokenNode* TokenQueue){
	assert(TokenQueue != NULL);
	TokenNode* head = TokenQueue;
	AST_node* root = create_node(head);
	while (head->next != NULL){
		if (is_type_command(head->token)){
			add_command_t add_command = add_command_map[head->token->type];
			head = add_command(head, TokenQueue, root);
		} else if (is_var(head, TokenQueue, root)) {
			head = add_var(head, root, root, TokenQueue);
		} else {
			warning(head->linenumber, "Espace dans le nom de variable ou fonctionalitée non prise en charge (macro ou autre). Ignorée: %s", head->token->lexem);
			head = head->next;
		}
	}
	return root;	
}

// int main(int argc, char** argv){
// 	FILE* f = fopen(argv[1], "r");
// 	TokenNode* tokenQueue = lexer(f);
// 	fclose(f);
// 	AST_node* root = parse(tokenQueue);
// 	print_AST(root);
// 	free_list(tokenQueue);
// 	free_AST(root);
// 	return 0;	
// }