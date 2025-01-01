#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"
#include "../lexer.h"
#include "../token.h"
#include "../AST.h"

void test_concat_and_slice() {
    printf("[Test] Test des fonctions concat et slice:\n");

    char* result = concat("Hello", "World");
    assert(strcmp(result, "HelloWorld") == 0);
    free(result);

    char* sliced = slice("HelloWorld", 0, 5);
    assert(strcmp(sliced, "Hello") == 0);
    free(sliced);

    printf("\t[OK] concat et slice ont passé les tests.\n");
}

void test_is_var() {
    printf("[Test] Test de is_var:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(IDENTIFIER, "var1");
    queue->linenumber = 1;
    queue->next = malloc(sizeof(TokenNode));
    queue->next->token = create_token(EQUAL, "=");
    queue->next->linenumber = 1;
    queue->next->next = malloc(sizeof(TokenNode));
    queue->next->next->token = create_token(NOTE, "a");
    queue->next->next->linenumber = 1;

    AST_node* root = create_node(queue);

    assert(is_var(queue, queue, root));
    printf("\t[OK] is_var a passé le test.\n");

    free_list(queue);
    free_AST(root);
}

void test_add_unknown() {
    printf("[Test] Test de add_unknown:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_UNKNOWN, "\\unknown_command");
    queue->linenumber = 1;
    queue->next = NULL;

    AST_node* root = create_node(queue);
    TokenNode* result = add_unknown(queue, root, queue, root);

    assert(result == queue->next);
    printf("\t[OK] add_unknown a passé le test.\n");

    free_list(queue);
    free_AST(root);
}

void test_add_relative() {
    printf("[Test] Test de add_relative:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_RELATIVE, "\\relative");
    queue->linenumber = 1;
    queue->next = malloc(sizeof(TokenNode));
    queue->next->token = create_token(NOTE, "c'4");
    queue->next->linenumber = 1;
    queue->next->next = malloc(sizeof(TokenNode));
    queue->next->next->token = create_token(LEFT_BRACE, "{");
    queue->next->next->linenumber = 1;
    queue->next->next->next = malloc(sizeof(TokenNode));
    queue->next->next->next->token = create_token(NOTE, "d'4");
    queue->next->next->next->linenumber = 1;
    queue->next->next->next->next = malloc(sizeof(TokenNode));
    queue->next->next->next->next->token = create_token(RIGHT_BRACE, "}");
    queue->next->next->next->next->linenumber = 1;
    queue->next->next->next->next->next = NULL;

    AST_node* root = create_node(queue);
    TokenNode* result = add_relative(queue, root, queue, root);

    assert(result == queue->next->next->next->next->next);
    printf("\t[OK] add_relative a passé le test.\n");

    free_list(queue);
    free_AST(root);
}

void test_add_note() {
    printf("[Test] Test de add_note:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(NOTE, "c'4");
    queue->linenumber = 1;
    queue->next = NULL;

    AST_node* root = create_node(queue);
    TokenNode* result = add_note(queue, root);

    assert(result == queue->next);
    printf("\t[OK] add_note a passé le test.\n");

    free_list(queue);
    free_AST(root);
}

void test_ignore_command() {
    printf("[Test] Test de ignore_command:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_UNKNOWN, "\\unknown");
    queue->linenumber = 1;
    queue->next = malloc(sizeof(TokenNode));
    queue->next->token = create_token(LEFT_BRACE, "{");
    queue->next->linenumber = 1;
    queue->next->next = malloc(sizeof(TokenNode));
    queue->next->next->token = create_token(RIGHT_BRACE, "}");
    queue->next->next->linenumber = 1;
    queue->next->next->next = NULL;

    AST_node* root = create_node(queue);
    TokenNode* result = ignore_command(queue, queue, root);

    assert(result == queue->next->next);
    printf("\t[OK] ignore_command a passé le test.\n");

    free_list(queue);
    free_AST(root);
}


void test_parse() {
    printf("[Test] Test de parse:\n");

    FILE *f = fopen("test_parse.txt", "w+");
    fprintf(f, "\\relative c'4 { d e f }\n");
    rewind(f);

    TokenNode* queue = lexer(f);
    fclose(f);

    AST_node* root = parse(queue);

    assert(root != NULL);
    print_AST(root);

    free_list(queue);
    free_AST(root);

    printf("\t[OK] parse a passé le test.\n");
}

void test_parser() {
    test_concat_and_slice();
    test_is_var();
    // test_add_unknown();
    // test_add_relative();
    // test_add_note();
    // test_ignore_command();
    test_parse();

    printf("\nTous les tests du parser sont terminés avec succès.\n");
}
