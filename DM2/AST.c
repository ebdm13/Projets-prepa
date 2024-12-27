#include "AST.h"
#include "error.h"
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

AST_node* create_AST_node(Token* token, int numberline) {
	assert(token != NULL);
	AST_node* node = malloc(sizeof(AST_node));
	node->token = token;
	node->numberline = numberline;
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

	if (root->token->type != BOT){
		if (root->next_sibling != NULL){
			printf("├── ");
		} else {
			printf("└── ");
			sticks[indent] = false;
		}
	}
	
	if (root->first_child != NULL){
		printf("%s\n", get_type(root->token));
		print_AST_node(root->first_child, indent + 1, sticks);
	} else {
		printf("%s: %s\n", get_type(root->token), root->token->lexem);
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
	AST_node* root = create_AST_node(create_token(BOT, ""), 0);
	AST_node* relative = create_AST_node(create_token(COMMAND_RELATIVE, "\\relative"));
	AST_node* relative_note = create_AST_node(create_token(NOTE, "c''"), 0);
	AST_node* key = create_AST_node(create_token(COMMAND_KEY, "\\key"), 0);
	AST_node* key_note = create_AST_node(create_token(NOTE, "a"), 0);
	AST_node* note_1 = create_AST_node(create_token(NOTE, "a4"), 0);
	AST_node* note_2 = create_AST_node(create_token(NOTE, "c2"), 0);
	AST_node* note_3 = create_AST_node(create_token(NOTE, "f'"), 0);
	AST_node* duree = create_AST_node(create_token(DUREE, "4"), 0);
	add_child(root, relative);
	add_child(relative, relative_note);
	add_child(relative, key);
	add_child(key, key_note);
	add_child(relative, note_1);
	add_child(relative, note_2);
	add_child(relative, note_3);
	add_child(note_3, duree);

	print_AST(root);
	AST_node* nodes[] = {root, relative, relative_note, key, key_note, note_1, note_2, note_3, duree};
	for (int i = 0; i < 9; i++){
		free_token(nodes[i]->token);
	}
	free_AST(root);
}

int main(){
	test_AST();
	return 0;
}
