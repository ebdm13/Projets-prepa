#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser.h"
#include "../lexer.h"
#include "../token.h"
#include "../AST.h"
#include "../error.h"

void test_concat() {
    printf("[Test] Test de concat:\n");

    char* result = concat("Hello", "World");
    assert(strcmp(result, "HelloWorld") == 0);
    free(result);

    info(0, "\t[✓] concat a passé le test.\n");
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
    queue->next->next->next = NULL;

    AST_node* root = create_node(queue);

    assert(is_var(queue, queue, root));
    info(0, "\t[✓] is_var a passé le test.\n");

    free_list(queue);
    free_AST(root);
}

void test_add_var() {
    printf("[Test] add_var:\n");

    // Création d'un contexte pour les tests
    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(IDENTIFIER, "var");
    queue->linenumber = 1;
    queue->next = NULL;
    TokenNode* head = add_token(queue, create_token(EQUAL, "="), 1);
    head = add_token(head, create_token(REST, "r4"), 1);
    head =add_token(head, create_token(RIGHT_BRACE, "}"), 1);
   
   Token* root_token = create_token(BOT, "");
    AST_node* root = create_AST_node(root_token, 0);

    TokenNode* result = add_var(queue, root, root, queue);
    assert(result == head); 
    assert(root->first_child->token->type == VAR_NAME);
    assert(root->first_child->first_child->token ==  queue->next->next->token);
    info(0, "\t[✓] add_var a passé le test.\n");

    free_token(root_token);
    free_list(queue);
    free_AST(root);
}

void test_is_var_defined() {
    printf("[Test] is_var_defined:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->linenumber = 0;
    queue->token = create_token(BOT, ""); 
    queue->next = NULL;
    AST_node* root = create_node(queue);
    add_token(queue, create_token(VAR_NAME, "var_name"), 1);
    add_token(queue->next, create_token(NOTE, "a,4"), 1);
    add_child(root, create_node(queue->next));
    add_child(root->first_child, create_node(queue->next->next));

    assert(is_var_defined(root, "\\var_name")); // Variable définie
    assert(!is_var_defined(root, "\\undefined_var")); // Variable non définie
    free_list(queue);
    free_AST(root);

    info(0, "\t[✓] is_var_defined a passé le test.\n");
}

void test_ignore_command() {
    printf("[Test] ignore_command:\n");

    // Création d'un contexte de test
    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_UNKNOWN, "\\unknown");
    queue->linenumber = 1;
    queue->next = NULL;
    TokenNode* head = add_token(queue, create_token(LEFT_BRACE, "{"), 1);
    head = add_token(head, create_token(NOTE, "a'32"), 1);
    head = add_token(head, create_token(RIGHT_BRACE, "}"), 1);
    head = add_token(head, create_token(EOT, ""), 2);

    Token* root_token = create_token(BOT, ""); 
    AST_node* root = create_AST_node(root_token, 0);
    TokenNode* result = ignore_command(queue, queue, root);

    assert(result->token == head->token);
    free_list(queue);
    free_token(root_token);
    free_AST(root);

    info(0, "\t[✓] ignore_command a passé le test.\n");
}

void test_add_note() {
    printf("[Test] add_note:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(NOTE, "bis'16");
    queue->linenumber = 1;
    queue->next = NULL;
    Token* root_token = create_token(NOTE, "a,4");
    AST_node* root = create_AST_node(root_token, 0);
    TokenNode* result = add_note(queue, root);

    AST_node* first_note = root->first_child;
    assert(first_note->token->type == NOTE);
    assert(result == NULL);
    assert(strcmp(first_note->token->lexem, "bis'16") == 0);

    AST_node* note_child = first_note->first_child;
    assert(note_child->token->type == NOTE_NAME);
    assert(strcmp(note_child->token->lexem, "b") == 0);
    note_child = note_child->next_sibling;
    assert(note_child->token->type == ACCIDENTAL);
    assert(strcmp(note_child->token->lexem, "is") == 0);
    note_child = note_child->next_sibling;
    assert(note_child->token->type == OCTAVE);
    assert(strcmp(note_child->token->lexem, "'") == 0);
    note_child = note_child->next_sibling;
    assert(note_child->token->type == DUREE);
    assert(strcmp(note_child->token->lexem, "16") == 0);

    free_token(root_token);
    free_list(queue);
    free_AST(root);
    info(0, "\t[✓] add_note a passé le test.\n");
}

void test_add_relative() {
    printf("[Test] add_relative:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(COMMAND_RELATIVE, "\\relative");
    queue->linenumber = 1;
    queue->next = NULL;
    TokenNode* head = add_token(queue, create_token(NOTE, "c'"), 1);
    head = add_token(head, create_token(LEFT_BRACE, "{"), 1);
    head = add_token(head, create_token(NOTE, "d'4"), 1);
    head = add_token(head, create_token(RIGHT_BRACE, "}"), 1);

    Token* root_token = create_token(BOT, ""); 
    AST_node* root = create_AST_node(root_token, 0);
    
    TokenNode* result = add_relative(queue, root, queue, root);

    assert(result == NULL);
    assert(root->first_child->token->type == COMMAND_RELATIVE);
    assert(root->first_child->first_child->token->type == NOTE);
    assert(strcmp(root->first_child->first_child->token->lexem, "c'") == 0);
    assert(root->first_child->first_child->next_sibling->token->type == NOTE);
    assert(strcmp(root->first_child->first_child->next_sibling->token->lexem, "d'4") == 0);
    free_token(root_token);
    free_list(queue);
    free_AST(root);
    info(0, "\t[✓] add_relative a passé le test.\n");    
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
    info(0, "\t[✓] lexer a passé le test.\n");   
}

void test_parser() {
    test_concat();
    test_is_var();
    test_is_var_defined();
    test_add_var();
    test_ignore_command();
    test_add_note();
    test_add_relative();
    test_parse();

    info(0, "\nTous les tests du parser sont terminés avec succès.\n");
}
