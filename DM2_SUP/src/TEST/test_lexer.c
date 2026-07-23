#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer.h"
#include "../token.h"
#include "../error.h"

void test_isCommand() {
    printf("[Test] Test de isCommand:\n");

    assert(isCommand("\\command"));
    assert(!isCommand("not_a_command"));
    assert(isCommand("\\<"));
    assert(!isCommand("\\Command"));

    info(0, "\t[✓] isCOmmand a passé le test.\n");
}

void test_isRest() {
    printf("[Test] Test de isRest:\n");

    assert(isRest("r1"));
    assert(isRest("r32"));
    assert(!isRest("note"));

    info(0, "\t[✓] isRest a passé le test.\n");
}

void test_isNote() {
    printf("[Test] Test de isNote:\n");

    assert(isNote("bis'16"));
    assert(isNote("ceses,,,4"));
    assert(isNote("c"));
    assert(isNote("g''"));
    assert(isNote("g32"));
    assert(isNote("g,,8"));
    assert(!isNote("r"));

    info(0, "\t[✓] isNote a passé le test.\n");
}

void test_isNumber() {
    printf("[Test] Test de isNumber:\n");

    assert(isNumber("123"));
    assert(!isNumber("12.5"));

    info(0, "\t[✓] isNumber a passé le test.\n");
}

void test_skip_comment() {
    printf("[Test] Test de skip_comment:\n");
    // Test 1: Commentaire en bloc
    {
        char* test_string = "{ This is \n a \n block \n comment %}";
        FILE* f = fopen("test_file1.txt", "w+");
        fprintf(f, "%s", test_string);
        rewind(f);
        int linenumber = 1;
        TokenNode* queu = malloc(sizeof(TokenNode));
        int result = skip_comment(f, &linenumber, queu);
        fclose(f);
        remove("test_file1.txt");
        free(queu);
        assert(result != EOF);
        assert(linenumber == 4);
    }

    // Test 2: Commentaire en ligne
    {
        const char* test_string = "% This is a line comment\n";
        FILE* f = fopen("test_file2.txt", "w+");
        fprintf(f, "%s", test_string);
        rewind(f);
        int linenumber = 1;
        TokenNode* queu = malloc(sizeof(TokenNode));
        int result = skip_comment(f, &linenumber, queu);
        fclose(f);
        free(queu);
        remove("test_file2.txt");
        assert(result != EOF);
        assert(linenumber == 2);
    } 
    info(0, "\t[✓] skip_comment a passé le test.\n");
}

void test_get_string() {
    printf("[Test] Test de get_string:\n");

    char* test_string = "This is a valid string\"";
    FILE* f = fopen("test_file1.txt", "w+");
    fprintf(f, "%s", test_string);
    rewind(f);
    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->next = NULL;
    queue->linenumber = 1;
    Token* result = get_string(f, 1, queue);
    fclose(f);
    remove("test_file1.txt");
    assert(strcmp(result->lexem, "\"This is a valid string\"") == 0);
    free_token(result);
    free(queue);

    info(0, "\t[✓] get_string a passé le test.\n");
}

void test_lexer() {
    printf("[Test] Test de lexer:\n");

    FILE *f = fopen("test_lexer.txt", "w+");
    fprintf(f, "\\command c'4 r4\n");
    rewind(f);

    TokenNode* queue = lexer(f);
    fclose(f);

    assert(queue != NULL);
    print_tokens(queue);
    free_list(queue);

    info(0, "\t[✓] lexer a passé le test.\n");
}

void main_test_lexer() {
    test_isCommand();
    test_isRest();
    test_isNote();
    test_isNumber();
    test_skip_comment();
    test_get_string();
    test_lexer();

    info(0, "\nTous les tests du lexer sont terminés avec succès.\n");
}
