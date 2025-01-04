#include "parser.h"
#include "lexer.h"	
#include "AST.h"
#include "error.h"
#include <float.h>
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
	assert(TokenQueue != NULL);
	assert(root != NULL);
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
			if (!isalnum(lexem[i]) && lexem[i] != '_' && lexem[i] != '.' && lexem[i] != '-') {
				error(head->linenumber, "Nom de variable non autorisé. Il ne peut contenir que des chiffres, des lettres et _: %s", lexem);
				clean_exit(TokenQueue, root);
			}
		}
		return true;
	} else {
		return false;
	}
}

// liste des fonction pour ajouter une command à l'AST
add_command_t add_command_map[] = {add_unknown, add_unknown, add_unknown, add_relative, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown, add_unknown};

TokenNode* add_context_element(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue){
	assert(head != NULL);
	assert(head->token->lexem != NULL);
	assert(node != NULL);
	if (head->token->type == EOT) {
		error(node->linenumber, "'}' manquant, ou valeur non définie: %s", node->token->lexem);
		clean_exit(TokenQueue, root);
	}
	if (is_type_command(head->token)){
		add_command_t add_command = add_command_map[head->token->type];
		return add_command(head, node, TokenQueue, root);
	}
	if (head->token->type == NOTE){
		return add_note(head, node);
	}
	add_child(node, create_node(head));
	return head->next;
}

TokenNode* add_var(TokenNode* head, AST_node* node, AST_node* root, TokenNode* TokenQueue){
	assert(head != NULL);
	assert(root != NULL);
	head->token->type = VAR_NAME;
	AST_node* var_node = create_node(head);
	add_child(node, var_node);
	head = head->next->next;
	if (head->token->type == LEFT_BRACE){
		head = head->next;
		while (head->token->type != RIGHT_BRACE){
			head = add_context_element(head, var_node, root, TokenQueue);
		}
		return head->next;
	} 
	return add_context_element(head, var_node, root, TokenQueue);
}

bool is_var_defined(AST_node *parent, char* name){
	assert(name != NULL);
	while (parent != NULL){
		for (AST_node* child = parent->first_child; child != NULL; child=child->next_sibling){
			if (child->token->type == VAR_NAME) {
				if (strcmp(child->token->lexem, name + 1) == 0 && child->first_child != NULL){
					return true;
				}
			}
		}
		parent = parent->parent;
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
			if (open_brace_cont < 0){
				return node;
			}
		}

		node = node->next;
	}

	if (open_brace_cont > 0){
		error(head->linenumber, "'}' manquante");
		clean_exit(TokenQueue, root);
	}

	return node;
}

TokenNode* add_unknown(TokenNode* head, AST_node* node, TokenNode *TokenQueue, AST_node* root){
	assert(head != NULL);
	if (is_var_defined(node, head->token->lexem)){
		add_child(node, create_node(head));
		return head->next;
	}
	warning(head->linenumber, "Cette commande n'existe pas, fait référence à une commande non-définie ou n'a pas encore été implémentée: %s", head->token->lexem);
	return ignore_command(head, TokenQueue, root);
}

TokenNode* add_note(TokenNode* head, AST_node* node){
	assert(head != NULL);
 	assert(node != NULL);
 	assert(head->token->type == NOTE);
 	assert(head->token->lexem != NULL);
 	char* lexem = head->token->lexem;
 	int len = strlen(lexem);
 	int linenumber = head->linenumber;
 	AST_node* note = create_node(head);
 	add_child(node, note);
 	char name[2] = {lexem[0], '\0'};
 	Token* note_name_token = create_token(NOTE_NAME, name);
 	head = insert_token(head, note_name_token, linenumber);
 	add_child(note, create_node(head));
 	int i = 1;
	if (len >= 5 && (strncmp(lexem + i , "isis", 4) == 0 || strncmp(lexem + i, "eses", 4) == 0)) {
		char accidental[5] = {lexem[i], lexem[i+1], lexem[i+2], lexem[i+3], '\0'};
		Token* accidental_token = create_token(ACCIDENTAL, accidental);
		head = insert_token(head, accidental_token, linenumber);
		add_child(note, create_node(head));
		i = 5;
 	} else if (len >= 3 && (strncmp(lexem + i, "is", 2) == 0 || strncmp(lexem + i, "es", 2) == 0)) {
 			char accidental[3] = {lexem[i], lexem[i+1], '\0'};
			Token* accidental_token = create_token(ACCIDENTAL, accidental);
			head = insert_token(head, accidental_token, linenumber);
 			add_child(note, create_node(head));
 			i = 3;
 	} 
 	int j = i;
	while (lexem[j] != '\0' && (lexem[j] == '\'' || lexem[j] == ',')) j++;
	if (j - i != 0) {
		char* octave = strndup(lexem + i, j - i);
		Token* octave_token = create_token(OCTAVE, octave);
		head = insert_token(head, octave_token, linenumber);
		add_child(note, create_node(head));
		free(octave);
	}
	if (len > j){
		char* duree = strndup(lexem + j, len - j);
		Token* duree_token = create_token(DUREE, duree);
		head = insert_token(head, duree_token, linenumber);
		add_child(note, create_node(head));
		free(duree);
	}
 	return head->next;
}

TokenNode* add_relative(TokenNode* head, AST_node* node, TokenNode* TokenQueue,AST_node* root){
	assert(head != NULL);
	assert(node != NULL);
	AST_node* relative = create_node(head);
	head = head->next;
	if (head->token->type == NOTE){
		head = add_note(head, relative);
		if (head->token->type == LEFT_BRACE){
			head = head->next;
			while (head->token->type != RIGHT_BRACE){
				if (is_var(head, TokenQueue, root)){
					head = add_var(head, relative, root, TokenQueue);
				} else {
					head = add_context_element(head, relative, root, TokenQueue);
				}
			}
			add_child(node, relative);
			return head->next;
		} else {
			error(relative->linenumber, "'{' manquante avant: %s", head->token->lexem);
			clean_exit(TokenQueue, root);
		}
	} else {
		error(relative->linenumber, "\\relative doit être suivit d'une note");
		clean_exit(TokenQueue, root);
	}
	return NULL;
}

AST_node* parse(TokenNode* TokenQueue){
	assert(TokenQueue != NULL);
	AST_node* root = create_node(TokenQueue);
	TokenNode* head = TokenQueue->next;
	while (head != NULL && head->token->type != EOT){
		if (is_type_command(head->token)){
			add_command_t add_command = add_command_map[head->token->type];
			head = add_command(head, root, TokenQueue, root);
		} else if (is_var(head, TokenQueue, root)) {
			head = add_var(head, root, root, TokenQueue);
		} else if (head->token->type == LEFT_BRACE) {
			head = head->next;
			while (head->token->type != RIGHT_BRACE){
				if (head->token->type == EOT){
					error(0, "'}' manquante");
					clean_exit(TokenQueue, root);
				}
				if (head->token->type == NOTE){
					head = add_note(head, root);
				} else if (head->token->type == REST) {
					add_child(root, create_node(head));
					head = head->next;
				} else {
					warning(head->linenumber, "%s n'est pas une note ni un silence", head->token->lexem);
				}
			}
			head = head->next;
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