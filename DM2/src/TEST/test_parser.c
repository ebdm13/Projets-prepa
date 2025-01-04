#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"
#include "../lexer.h"
#include "../token.h"
#include "../AST.h"

void test_concat() {
    printf("[Test] Test des fonctions concat et slice:\n");

    char* result = concat("Hello", "World");
    assert(strcmp(result, "HelloWorld") == 0);
    free(result);

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
    test_concat();
    test_is_var();
    test_parse();

    printf("\nTous les tests du parser sont terminés avec succès.\n");
}
