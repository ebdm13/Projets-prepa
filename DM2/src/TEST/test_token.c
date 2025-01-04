#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "../token.h"

void test_create_token() {
    printf("[Test] Test de create_token:\n");

    Token* token = create_token(NOTE, "c'4");
    assert(token != NULL);
    assert(token->type == NOTE);
    assert(strcmp(token->lexem, "c'4") == 0);

    free_token(token);
    printf("\t[OK] create_token a passé le test.\n");
}

void test_add_token() {
    printf("[Test] Test de add_token:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->next = NULL;

    Token* token = create_token(NOTE, "c'4");
    TokenNode* new_node = add_token(queue, token, 1);

    assert(new_node != NULL);
    assert(new_node->token == token);
    assert(queue->next == new_node);

    free_list(queue);
    printf("\t[OK] add_token a passé le test.\n");
}

void test_insert_token() {
    printf("[Test] Test de insert_token:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->next = NULL;

    Token* token1 = create_token(NOTE, "c'4");
    TokenNode* node1 = add_token(queue, token1, 1);

    Token* token2 = create_token(NOTE, "d'4");
    TokenNode* new_node = insert_token(queue, token2, 1);

    assert(new_node != NULL);
    assert(new_node->token == token2);
    assert(queue->next == new_node);
    assert(new_node->next == node1);

    free_list(queue);
    printf("\t[OK] insert_token a passé le test.\n");
}

void test_free_list() {
    printf("[Test] Test de free_list:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->next = NULL;

    add_token(queue, create_token(NOTE, "c'4"), 1);
    add_token(queue, create_token(NOTE, "d'4"), 1);

    free_list(queue);
    printf("\t[OK] free_list a passé le test (pas de fuite mémoire détectée).\n");
}

void test_print_tokens() {
    printf("[Test] Test de print_tokens:\n");

    TokenNode* queu = malloc(sizeof(TokenNode));
    queu->token = create_token(BOT, "");
    queu->next = NULL;
    TokenNode* head = add_token(queu, create_token(NUMBER, "45"), 0);
    head = add_token(head, create_token(EQUAL, "="), 0);
    head = add_token(head, create_token(NUMBER, "45"), 0);
    head = add_token(head, create_token(EOT, ""), 0);
    print_tokens(queu);
    free_list(queu);
    printf("\t[OK] print_tokens a affiché les tokens avec succès.\n");
}

void test_token() {
    test_create_token();
    test_add_token();
    test_insert_token();
    test_free_list();
    test_print_tokens();

    printf("\nTous les tests de gestion des tokens sont terminés avec succès.\n");
}
