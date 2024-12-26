#include "AST.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

AST_node* create_AST_node(TokenType type, char* lexem) {
	assert(lexem != NULL);
	AST_node* node = malloc(sizeof(AST_node));
	Token token = {type, lexem} ;
	node->value = token;
	node->parent = NULL;
	node->first_child = NULL;
	node->next_sibling = NULL;
	return node;
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

void print_AST_node(AST_node* root, int indent, bool sticks[100]){
	if (indent >= 100) {
		error("Arbre trop profond, il y a trop de noeuds enfants.", 0);
	}

	if (root == NULL){
		return;
	}

	for (int i = 1; i < indent; i++){
		if (sticks[i] == true) {
			printf("│    ");
		} else {
			printf("     ");
		}
	}

	if (root->value.type != BOT){
		if (root->next_sibling != NULL){
			printf("├── ");
		} else {
			printf("└── ");
			sticks[indent] = false;
		}
	}
	
	if (root->first_child != NULL){
		printf("%s\n", get_type(root->value));
		print_AST_node(root->first_child, indent + 1, sticks);
	} else {
		printf("%s: %s\n", get_type(root->value), root->value.lexem);
	}

	if (root->next_sibling != NULL){
		print_AST_node(root->next_sibling, indent, sticks);
	}
}

void print_AST(AST_node *root){
	assert(root != NULL);
	bool sticks[100];
	for (int i = 0; i < 100; i++){
		sticks[i] = true;
	}
	print_AST_node(root, 0, sticks);
}

void test_AST(){
	AST_node* root = create_AST_node(BOT, "");
	AST_node* relative = create_AST_node(COMMAND_RELATIVE, "\\relative");
	AST_node* relative_note = create_AST_node(NOTE, "c''");
	AST_node* key = create_AST_node(COMMAND_KEY, "\\key");
	AST_node* key_note = create_AST_node(NOTE, "a");
	AST_node* note_1 = create_AST_node(NOTE, "a4");
	AST_node* note_2 = create_AST_node(NOTE, "c2");
	AST_node* note_3 = create_AST_node(NOTE, "f'");
	AST_node* duree = create_AST_node(DUREE, "4");
	add_child(root, relative);
	add_child(relative, relative_note);
	add_child(relative, key);
	add_child(key, key_note);
	add_child(relative, note_1);
	add_child(relative, note_2);
	add_child(relative, note_3);
	add_child(note_3, duree);

	print_AST(root);
	free_AST(root);
}