#pragma once
#include "sound.h"
#include "AST.h"

typedef struct {
	int  pitch;
	int duree;
} note_t;

/*Renvoie un pointeur vers un note_t de pitch et duree correspondant*/
note_t* create_note(int pitch, int duree);

/*Convertie un noeud de l'AST en note_t. Attention, le noeud note doit être correct, ce qui est le cas si il est généré par le parser.*/
note_t* render_note(AST_node* note);

/*Renvoi la duree en seconde d'une note. En fonction du bpm: batement par minute, de duree_ref, la duree de référence. Ex: duree_ref = 4 et bpm=120 -> 120 noirs par minutes*/
float duree_to_second(int duree, int duree_ref, int bpm);

/*Génère le son correspondant à l'AST (de racine root)*/
track_t* render(AST_node* root, int f_ech, int duree_ref, int bpm);
