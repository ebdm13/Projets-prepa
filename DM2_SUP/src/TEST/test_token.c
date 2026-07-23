#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "../token.h"
#include "../error.h"

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

    Token* token = create_token(REST, "r16");
    TokenNode* new_node = add_token(queue, token, 1);

    assert(new_node != NULL);
    assert(new_node->token == token);
    assert(queue->next == new_node);

    free_list(queue);
    info(0, "\t[✓] add_token a passé le test.\n");
}

void test_insert_token() {
    printf("[Test] Test de insert_token:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->next = NULL;

    Token* token1 = create_token(COMMAND_RELATIVE, "\\relative");
    TokenNode* node1 = add_token(queue, token1, 1);

    Token* token2 = create_token(NOTE, "d'4");
    TokenNode* new_node = insert_token(queue, token2, 1);

    assert(new_node != NULL);
    assert(new_node->token == token2);
    assert(queue->next == new_node);
    assert(new_node->next == node1);

    free_list(queue);
    info(0, "\t[✓] insert_token a passé le test.\n");
}

void test_get_type(){
    Token* token = create_token(COMMAND_HEADER, "\\header");
    assert(strcmp(get_type(token), "COMMAND_HEADER") == 0);
    free_token(token);
    token = create_token(TIE, "~");
    assert(strcmp(get_type(token), "TIE") == 0);
    free_token(token);
    token = create_token(STRING, "\" coucou \"");
    assert(strcmp(get_type(token), "STRING") == 0);
    free_token(token);
    info(0, "\t[✓] get_type a passé le test.\n");
}

void test_is_type_command(){
    Token* token = create_token(COMMAND_L_ANGLE, "\\<");
    assert(is_type_command(token));
    free_token(token);
    token = create_token(VAR_CALL, "\\coucou");
    assert(!is_type_command(token));
    free_token(token);
    info(0, "\t[✓] is_type_command a passé le test.\n");
}

void test_print_tokens() {
    printf("[Test] Test de print_tokens:\n");

    TokenNode* queu = malloc(sizeof(TokenNode));
    queu->token = create_token(BOT, "");
    queu->linenumber = 0;
    queu->next = NULL;
    TokenNode* head = add_token(queu, create_token(NUMBER, "45"), 1);
    head = add_token(head, create_token(EQUAL, "="), 1);
    head = add_token(head, create_token(NUMBER, "45"), 1);
    head = add_token(head, create_token(EOT, ""), 2);
    print_tokens(queu);
    free_list(queu);
    info(0, "\t[✓] print_tokens a affiché les tokens avec succès.\n");
}

void test_token() {
    test_create_token();
    test_add_token();
    test_insert_token();
    test_get_type();
    test_is_type_command();
    test_print_tokens();

    info(0, "\nTous les tests de gestion des tokens sont terminés avec succès.\n");
}
