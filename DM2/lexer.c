#include "lexer.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>

bool isCommand(char* s){
	assert(s != NULL);
	if (s[0] == '\\' && strlen(s) > 1) {
		if ((s[1] == '<' || s[1] == '>') && strlen(s) == 2) {
			return true;
		}
		for (int i = 1; s[i] != '\0'; i++){
			// if s[i] is not lower
			if (s[i] < 97 || s[i] > 122) {
				return false;
			}
		}
		return true;
	}
	return false;
}

bool isRest(char* s){
	assert(s != NULL);
	int len = strlen(s);
	if (len == 2 && s[0] == 'r' && (s[1] == '1' || s[1] == '2' || s[1] == '4' || s[1] == '8')) return true;
	if (strcmp(s, "r16") == 0 || strcmp(s, "r32") == 0 || strcmp(s, "r64") == 0) return true;
	return false;
}

bool isNumber(char* s){
	assert(s != NULL);
	for (int i = 0; s[i] != '\0'; i++){
		if (s[i] < 48 || s[i] > 57) return false;
	}
	return true;
}

int skip_comment(FILE* f, int* linenumber, TokenNode* queu){
	assert(linenumber != NULL);
	if (f == NULL) {
		error(*linenumber, "Fichier invalide");
		free_list(queu);
		exit(EXIT_FAILURE);
	}
	char c;
	int a = fscanf(f, "%c", &c);
	// Comment Block
	if (c == '{') {
		char c2;
		a = fscanf(f, "%c", &c2);
		while (a != EOF && (c != '%' || c2 != '}')) {
			if (c2 == '\n') (*linenumber)++;
			c = c2;
			a = fscanf(f, "%c", &c2);
		}
	// Line Comment
	} else {
		while ((a=fscanf(f, "%c", &c)) != EOF && c != '\n');
		(*linenumber)++;
	}
	return a;	
}

Token* get_string(FILE* f, int linenumber, TokenNode* queue){
	assert(queue != NULL);
	if (f == NULL) {
		error(linenumber, "Fichier invalide");
		free_list(queue);
		exit(EXIT_FAILURE);
	}

	char string[1024];
	char c;
	int a = fscanf(f, "%c", &c);
	string[0] = '"';
	int i = 1;
	while (c != '"'){
		if (a == EOF || c == '\n'){
			error(linenumber, "\" manquant");
			free_list(queue);
			exit(EXIT_FAILURE);
		}
		string[i++] = c;
		a = fscanf(f, "%c", &c);
	}
	string[i] = '"';
	string[i+1] = '\0';
	return create_token(STRING, string);
}

TokenType get_command_type(char* command){
	assert(command != NULL);
	if (strcmp(command, "\\version") == 0) return COMMAND_VERSION;
    if (strcmp(command, "\\header") == 0) return COMMAND_HEADER;
    if (strcmp(command, "\\relative") == 0) return COMMAND_RELATIVE;
    if (strcmp(command, "\\clef") == 0) return COMMAND_CLEF;
    if (strcmp(command, "\\key") == 0) return COMMAND_KEY;
    if (strcmp(command, "\\major") == 0) return COMMAND_MAJOR;
    if (strcmp(command, "\\minor") == 0) return COMMAND_MINOR;
    if (strcmp(command, "\\time") == 0) return COMMAND_TIME;
    if (strcmp(command, "\\repeat") == 0) return COMMAND_REPEAT;
    if (strcmp(command, "\\ppp") == 0) return COMMAND_PPP;
    if (strcmp(command, "\\pp") == 0) return COMMAND_PP;
    if (strcmp(command, "\\p") == 0) return COMMAND_P;
    if (strcmp(command, "\\mp") == 0) return COMMAND_MP;
    if (strcmp(command, "\\mf") == 0) return COMMAND_MF;
    if (strcmp(command, "\\f") == 0) return COMMAND_F;
    if (strcmp(command, "\\ff") == 0) return COMMAND_FF;
    if (strcmp(command, "\\fff") == 0) return COMMAND_FFF;
    if (strcmp(command, "\\fp") == 0) return COMMAND_FP;
    if (strcmp(command, "\\sf") == 0) return COMMAND_SF;
    if (strcmp(command, "\\sfp") == 0) return COMMAND_SFP;
    if (strcmp(command, "\\sfz") == 0) return COMMAND_SFZ;
    if (strcmp(command, "\\rfz") == 0) return COMMAND_RFZ;
    if (strcmp(command, "\\accent") == 0) return COMMAND_ACCENT;
    if (strcmp(command, "\\tenuto") == 0) return COMMAND_TENUTO;
    if (strcmp(command, "\\staccato") == 0) return COMMAND_STACCATO;
    if (strcmp(command, "\\marcato") == 0) return COMMAND_MARCATO;
    if (strcmp(command, "\\>") == 0) return COMMAND_R_ANGLE;
    if (strcmp(command, "\\<") == 0) return COMMAND_L_ANGLE;
    if (strcmp(command, "\\!") == 0) return COMMAND_EX_POINT;
    if (strcmp(command, "\\mordent") == 0) return COMMAND_MORDENT;
    if (strcmp(command, "\\appoggiatura") == 0) return COMMAND_APPOGGIATURA;
    if (strcmp(command, "\\prall") == 0) return COMMAND_PRALL;
    if (strcmp(command, "\\fermata") == 0) return COMMAND_FERMATA;
    if (strcmp(command, "\\turn") == 0) return COMMAND_TURN;
    if (strcmp(command, "\\trill") == 0) return COMMAND_TRILL;
    if (strcmp(command, "\\bar") == 0) return COMMAND_BAR;
    if (strcmp(command, "\\partial") == 0) return COMMAND_PARTIAL;
    return COMMAND_UNKNOWN;
}

Token* get_command(char* command){
	assert(command != NULL);
	return create_token(get_command_type(command), command);
}

Token* get_rest(char* rest){
	assert(rest != NULL);
	return create_token(REST, rest);
}

Token* get_token(char* buffer){
	assert(buffer != NULL);
	if (isCommand(buffer)) return get_command(buffer);
	if (isRest(buffer)) return get_rest(buffer);
	if (isNumber(buffer)) return create_token(NUMBER, buffer);
	return create_token(IDENTIFIER, buffer);
}

TokenNode* lexer(FILE* f){
	if (f == NULL) {
		error(0, "Fichier invalide");
		exit(EXIT_FAILURE);
	}

	TokenNode* tokenQueue = malloc(sizeof(TokenNode));
	tokenQueue->token = create_token(BOT, "");
	tokenQueue->next = NULL;
	tokenQueue->linenumber = 0;
	char c;
	char buffer[100] = {0};
	int linenumber = 1;
	TokenNode* head = tokenQueue;
	Token* newToken = NULL;
	bool endOfWord = false;
	int i = 0;
	int a = 0;
	while (a != EOF){
		a = fscanf(f, "%c", &c);
		if (c == ' ' || c == '\t' || c == '\v') {
			endOfWord = true;
		} else if (c == '\n') {
			linenumber++;
			endOfWord = true;
		} else if (c == '%'){
			a = skip_comment(f, &linenumber, tokenQueue);
			endOfWord = true;
		} else if (c == '"') {
			if (strlen(buffer) != 0) {
				error(linenumber, "espace manquant après %s", buffer);
				free_list(tokenQueue);
				exit(EXIT_FAILURE);
			}
			newToken = get_string(f, linenumber, tokenQueue);
			endOfWord = true;
		} else if (c == '(' || c == ')' || c == '#' || c == '}' || c == '{' || c == '\\' || c == '~' || c == '=' || c == '|') {
			switch (c) {
            	case '(': newToken = create_token(LEFT_PAREN, "("); break;
            	case ')': newToken = create_token(RIGHT_PAREN, ")"); break;
            	case '{': newToken = create_token(LEFT_BRACE, "{"); break;
            	case '}': newToken = create_token(RIGHT_BRACE, "}"); break;
            	case '#': newToken = create_token(HASH, "#"); break;
            	case '~': newToken = create_token(TIE, "~"); break;
            	case '\\': newToken = create_token(BACKSLASH, "\\"); break;
            	case '=': newToken = create_token(EQUAL, "="); break;
            	case '|': newToken = create_token(BARLINE, "|"); break;
            	default: break;
            }
            endOfWord = true;
        }

		if (endOfWord) {
			if (strlen(buffer) != 0) {
				head = add_token(head, get_token(buffer), linenumber);
			}

			endOfWord = false;
			i = 0;
			buffer[0] = '\0';

			if (newToken != NULL){
				if (newToken->type == BACKSLASH){
					free_token(newToken);
					buffer[0] = '\\';
					buffer[1] = '\0';
					newToken = NULL;
					i = 1;
				} else {
					head = add_token(head, newToken, linenumber);
					newToken = NULL;
				}
			}
		} else {
			if (i < 99) {
				buffer[i++] = c;
				buffer[i] = '\0';
			} else {
				error(linenumber, "Buffer overflow, ce mot est trop long: %s...", buffer);
				free_list(tokenQueue);
				exit(EXIT_FAILURE);
			}
		}
	}
	head = add_token(head, create_token(EOT, ""), linenumber);
	return tokenQueue;
}

// int main(int argc, char** argv){
// 	FILE* f = fopen(argv[1], "r");
// 	TokenNode* tokenQueue = lexer(f);
// 	fclose(f);
// 	print_tokens(tokenQueue);
// 	free_list(tokenQueue);
// 	return 0;	
// }