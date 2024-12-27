#include "parser.h"
#include "lexer.h"	
#include "AST.h"
#include "error.h"
#include <assert.h>

TokenNode* ignore_command(TokenNode* head);

 TokenNode* add_unknown(TokenNode* head, AST_node* root);

// liste des fonction pour ajouter une command à l'AST
add_command_t add_command_map[] = {add_unknown};

AST_node* parse(TokenNode* TokenQueue){
	assert(TokenQueue != NULL);
	TokenNode* head = TokenQueue;
	AST_node* root = create_AST_node(head->token);
	while (head->next != NULL){
		head = head->next;
		if (is_type_command(head->token)){
			add_command_t add_command = add_command_map[head->token->type];
			add_command(head, root);
		} else if (is_var(head)) {
			add_var(head, root);
		} else {
			warning("Espace dans le nom de variable ou fonctionalitée non prise en charge (macro ou autre). Ignorée.", head->linenumber);
		}
	}
	return root;	
}

int main(int argc, char** argv){
	FILE* f = fopen(argv[1], "r");
	TokenNode* tokenQueue = lexer(f);
	AST_node* root = parse(tokenQueue);
	fclose(f);
	print_AST(root);
	free_list(tokenQueue);
	free_AST(root);
	return 0;	
}