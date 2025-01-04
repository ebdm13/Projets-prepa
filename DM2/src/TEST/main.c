#include <stdio.h>

void test_token();
void test_AST();
void main_test_lexer();
void test_parser();

int main() {
    printf("========== Début des tests ==========\n");

    printf("\n[Module] Tests de token.c\n");
    test_token();
  
    printf("\n[Module] Tests de AST.c\n");
    test_AST();

    printf("\n[Module] Tests de lexer.c\n");
    main_test_lexer();

    printf("\n[Module] Tests de parser.c\n");
    test_parser();

    printf("\n========== Tous les tests sont terminés ==========\n");
    return 0;
}
