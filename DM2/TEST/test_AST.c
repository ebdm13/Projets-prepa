#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../AST.h"
#include "../token.h"

void test_create_AST_node() {
    printf("[Test] Test de create_AST_node:\n");

    Token* token = create_token(NOTE, "c'4");
    AST_node* node = create_AST_node(token, 1);

    assert(node != NULL);
    assert(node->token == token);
    assert(node->linenumber == 1);
    assert(node->parent == NULL);
    assert(node->first_child == NULL);
    assert(node->next_sibling == NULL);

    free_token(token);
    free(node);
    printf("\t[OK] create_AST_node a passé le test.\n");
}

void test_add_child() {
    printf("[Test] Test de add_child:\n");

    AST_node* parent = create_AST_node(create_token(COMMAND_RELATIVE, "\\relative"), 1);
    AST_node* child1 = create_AST_node(create_token(NOTE, "c'4"), 1);
    AST_node* child2 = create_AST_node(create_token(NOTE, "d'4"), 1);

    add_child(parent, child1);
    add_child(parent, child2);

    assert(parent->first_child == child1);
    assert(child1->next_sibling == child2);
    assert(child2->next_sibling == NULL);
    assert(child1->parent == parent);
    assert(child2->parent == parent);

    free_AST(parent);
    printf("\t[OK] add_child a passé le test.\n");
}

void test_free_AST() {
    printf("[Test] Test de free_AST:\n");

    AST_node* root = create_AST_node(create_token(BOT, ""), 0);
    AST_node* child1 = create_AST_node(create_token(NOTE, "c'4"), 1);
    AST_node* child2 = create_AST_node(create_token(NOTE, "d'4"), 1);
    add_child(root, child1);
    add_child(root, child2);

    free_AST(root);

    printf("\t[OK] free_AST a libéré la mémoire sans fuite.\n");
}

void test_print_AST() {
    printf("[Test] Test de print_AST:\n");

    AST_node* root = create_AST_node(create_token(BOT, ""), 0);
    AST_node* relative = create_AST_node(create_token(COMMAND_RELATIVE, "\\relative"), 1);
    AST_node* note = create_AST_node(create_token(NOTE, "c'4"), 1);

    add_child(root, relative);
    add_child(relative, note);

    print_AST(root);

    free_AST(root);
    printf("\t[OK] print_AST a affiché l'arbre avec succès.\n");
}

void test_AST() {
    test_create_AST_node();
    test_add_child();
    test_free_AST();
    test_print_AST();

    printf("\nTous les tests de l'AST sont terminés avec succès.\n");
}
