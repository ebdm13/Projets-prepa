#pragma once
#include<stdbool.h>

typedef enum {
	COMMAND_UNKNOWN,
	COMMAND_VERSION,
	COMMAND_HEADER,
	COMMAND_RELATIVE,
	COMMAND_CLEF,
	COMMAND_KEY,
	COMMAND_MAJOR,
	COMMAND_MINOR,
	COMMAND_TIME,
	COMMAND_REPEAT,
	// Dynamiques / Nuances
	COMMAND_PPP,
	COMMAND_PP,
	COMMAND_P,
	COMMAND_MP,
	COMMAND_MF,
	COMMAND_F,
	COMMAND_FF,
	COMMAND_FFF,
	COMMAND_FP,
	COMMAND_SF,
	COMMAND_SFP,
	COMMAND_SFZ,
	COMMAND_RFZ,
	// Points d'Articulation
	COMMAND_ACCENT,
	COMMAND_TENUTO,
	COMMAND_STACCATO,
	COMMAND_MARCATO,
	// Ornements
	// \>
	COMMAND_R_ANGLE,
	// \<
	COMMAND_L_ANGLE,
	// \!
	COMMAND_EX_POINT,
	COMMAND_MORDENT,
	COMMAND_APPOGGIATURA,	
	COMMAND_PRALL,
	COMMAND_FERMATA,
	COMMAND_TURN,
	COMMAND_TRILL,
	// Autres
	COMMAND_BAR,
	COMMAND_PARTIAL,
	VAR_NAME,
	VAR_CALL,
	REST,
	// a b c d e f g
	NOTE,
	// 1 2 4 8 16 32 64
	DUREE,
	// is es ih eh
	ACCIDENTAL,
	// '
	OCTAVE_UP,
	// ,
	OCTAVE_DOWN,
	// ~
	TIE,
	// .
	DOT,
	// |
	BARLINE,
	// <
	LEFT_ANGLE,
	// >
	RIGHT_ANGLE,
	IDENTIFIER,
	// =
	EQUAL,
	// Ex: 7
	NUMBER,
	// )
	RIGHT_PAREN,
	// (
	LEFT_PAREN,
	// {
	LEFT_BRACE,
	// }
	RIGHT_BRACE,
	// "truc"
	STRING,
	// #
	HASH,	
	BACKSLASH,
	// Begininf of Tokens
	BOT,
	// End Of Tokens
	EOT,
} TokenType;

typedef struct {
	TokenType type;
	char* lexem;
} Token;

/*Structure dans le but de faire une liste chainée FIFO de Token. On ajoutera les éléments par la tête de la liste, et on lira la liste en commencant par la queue*/
typedef struct TokenNode {
    Token* token;
    struct TokenNode* next;
    int linenumber;
} TokenNode;

/*Libère la mémoire alouée pour t*/
void free_token(Token* t);

/*renvoi un poiteur vers le token crée en fonction de son type et de son lexem*/
Token* create_token(TokenType type, char* lexem);

/*Ajoute le token à la liste en le reliant à head en renvoi un pointeur vers la nouvelle head*/
TokenNode* add_token(TokenNode* head, Token* token, int linenumber);

/*Renvoie le type du token en str*/
char* get_type(Token* token);

/*Vérifie si un token est de type COMMAND_*/
bool is_type_command(Token* token);

/*affiche le type et le lexem d'un token*/
void print_token(Token* token);

/*Affiche la liste des tokens*/
void print_tokens(TokenNode* queue);

/*Libère la mémoire alouée pour la liste de token*/
void free_list(TokenNode* queue);
