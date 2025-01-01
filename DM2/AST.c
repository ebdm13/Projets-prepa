#include "AST.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

AST_node* create_AST_node(Token* token, int linenumber) {
	assert(token != NULL);
	AST_node* node = malloc(sizeof(AST_node));
	node->token = token;
	node->linenumber = linenumber;
	node->parent = NULL;
	node->first_child = NULL;
	node->next_sibling = NULL;
	return node;
}

AST_node* create_node(TokenNode *node){
	return create_AST_node(node->token, node->linenumber);
}

void add_child(AST_node* parent, AST_node* child){
	assert(parent != NULL);
	assert(child != NULL);
	child->parent = parent;
	if (parent->first_child == NULL) {
		parent->first_child = child;
	} else {
		AST_node* next_child = parent->first_child;
		while (next_child->next_sibling != NULL){
			next_child = next_child->next_sibling;
		}
		next_child->next_sibling = child;
	}
}

void free_AST(AST_node* root){
	if (root == NULL) {
		return;
	}
	if (root->first_child != NULL) {
		free_AST(root->first_child);
	}
	if (root->next_sibling != NULL){
		free_AST(root->next_sibling);
	}

	free(root);
}

void print_AST_node(AST_node* node, AST_node* root, int indent, bool sticks[100]){
	if (indent >= 100) {
		int linenumber = node->linenumber;
		error(linenumber, "Arbre trop profond, il y a trop de noeuds enfants.");
		free_AST(root);
		exit(EXIT_FAILURE);
	}

	if (node == NULL){
		return;
	}

	for (int i = 1; i < indent; i++){
		if (sticks[i] == true) {
			printf("│    ");
		} else {
			printf("     ");
		}
	}

	if (node->token->type != BOT){
		if (node->next_sibling != NULL){
			printf("├── ");
			sticks[indent] = true;
		} else {
			printf("└── ");
			sticks[indent] = false;
		}
	}
	
	if (node->first_child != NULL){
		printf("%s\n", get_type(node->token));
		print_AST_node(node->first_child, root, indent + 1, sticks);
	} else {
		printf("%s: %s\n", get_type(node->token), node->token->lexem);
	}

	if (node->next_sibling != NULL){
		print_AST_node(node->next_sibling, root, indent, sticks);
	}
}

void print_AST(AST_node *root){
	assert(root != NULL);
	bool sticks[100];
	for (int i = 0; i < 100; i++){
		sticks[i] = true;
	}
	print_AST_node(root, root,0, sticks);
}
