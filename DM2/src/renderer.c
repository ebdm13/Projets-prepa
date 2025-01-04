#include "renderer.h"
#include "AST.h"
#include "token.h"
#include "error.h"
#include "waveform.h"
#include "sound.h"
#include "melody.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

note_t* create_note(int pitch, int duree){
	note_t* note = malloc(sizeof(note_t));
	note->pitch = pitch;
	note->duree = duree;
	return note;
}

float duree_to_second(int duree, int duree_ref, int bpm){
	return 60 * (float)duree_ref / (float)duree / (float)bpm;
}

note_t* render_note(AST_node* note){
	assert(note != NULL);
	assert(note->token->type == NOTE);
	int pitch = 0;
	int duree = 4;
	AST_node* node = note->first_child;
	char note_name = node->token->lexem[0];
	switch (note_name){
		case 'a': pitch = 0; break;
		case 'b': pitch = 2; break;
		case 'c': pitch = -9; break;
		case 'd': pitch = -7; break;
		case 'e': pitch = -5; break;
		case 'f': pitch = -4; break;
		case 'g': pitch = -2; break;
	}
	node = node->next_sibling;
	
	while (node != NULL){
		if (node->token->type == ACCIDENTAL){
			if (strcmp(node->token->lexem, "is") == 0) pitch++;
			else if (strcmp(node->token->lexem, "es") == 0) pitch--;
			else if (strcmp(node->token->lexem, "isis") == 0) pitch += 2;
			else if (strcmp(node->token->lexem, "eses") == 0) pitch -= 2;
		} else if (node->token->type == OCTAVE) {
			for (int i = 0; node->token->lexem[i] != '\0'; i++){
				if (node->token->lexem[i] == '\'') pitch += 12;
				else if (node->token->lexem[i] == ',') pitch -=12;
			}
		} else if (node->token->type == DUREE) {
			duree = atoi(node->token->lexem);
		}
		node = node->next_sibling;
	}
	return create_note(pitch, duree);
}

track_t* render(AST_node* root, int f_ech, int duree_ref, int bpm){
	track_t* track = malloc(sizeof(track_t));
	track->n_sounds = 0;
	AST_node* node = root->first_child;
	while (node != NULL){
		if (node->token->type == NOTE || node->token->type == REST){
			track->n_sounds++;
		}
		node = node->next_sibling;
	}
	track->sounds = malloc(track->n_sounds * sizeof(sound_t*));
	node = root->first_child;
	for (int i = 0; node != NULL; i++){
		if (node->token->type == NOTE){
			note_t* note = render_note(node);
			track->sounds[i] = sine(pitch_to_freq(note->pitch), 1000, duree_to_second(note->duree, duree_ref, bpm), f_ech);
			free(note);
		} else if (node->token->type == REST){
			track->sounds[i] = sine(0, 0, duree_to_second(atoi(node->token->lexem + 1), duree_ref, bpm), f_ech);
		} else {
			warning(node->linenumber, "%s: Non supporté et ignoré", node->token->lexem);
		}
		node = node->next_sibling;
	}
	return track;
}
