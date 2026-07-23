#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../AST.h"
#include "../token.h"
#include "../error.h"

void test_create_AST_node() {
    printf("[Test] Test de create_AST_node:\n");

    Token* token = create_token(NOTE, "c'16");
    AST_node* node = create_AST_node(token, 1);

    assert(node != NULL);
    assert(node->token == token);
    assert(node->linenumber == 1);
    assert(node->parent == NULL);
    assert(node->first_child == NULL);
    assert(node->next_sibling == NULL);

    free_token(token);
    free(node);
    info(0, "\t[✓] create_AST_node a passé le test.\n");
}

void test_add_child() {
    printf("[Test] Test de add_child:\n");
    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_RELATIVE, "\\relative");
    queue->linenumber = 1;
    queue->next = NULL;
    add_token(queue, create_token(NOTE, "c'4"), 1);
    add_token(queue->next, create_token(NOTE, "d'4"), 1);
    AST_node* parent = create_node(queue);
    AST_node* child1 = create_node(queue->next);
    AST_node* child2 = create_node(queue->next->next);

    add_child(parent, child1);
    add_child(parent, child2);

    assert(parent->first_child == child1);
    assert(child1->next_sibling == child2);
    assert(child2->next_sibling == NULL);
    assert(child1->parent == parent);
    assert(child2->parent == parent);

    free_list(queue);
    free_AST(parent);
    info(0, "\t[✓] add_child a passé le test.\n");

}

void test_print_AST() {
    printf("[Test] Test de print_AST:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->linenumber = 0;
    queue->next = NULL;
    TokenNode* head = add_token(queue, create_token(COMMAND_RELATIVE, "\\relative"), 1);
    add_token(head, create_token(NOTE, "c'4"), 1);
    AST_node* root = create_node(queue);

    add_child(root, create_node(queue->next));
    add_child(root->first_child, create_node(head));

    print_AST(root);

    free_list(queue);
    free_AST(root);
    info(0, "\t[✓] print_AST a passé le test.\n");

}

void test_AST() {
    test_create_AST_node();
    test_add_child();
    test_print_AST();

    info(0, "\nTous les tests de l'AST sont terminés avec succès.\n");
}
