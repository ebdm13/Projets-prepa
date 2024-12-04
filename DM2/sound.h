#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct {
	int n_samples;
	int16_t* samples;
} sound_t;

typedef struct {
	int n_sounds;
	sound_t** sounds;
} track_t;

/*Libère la mémoire alouée pour s*/
void free_sound(sound_t* s);

/*Libère la mémoire alouée pour t*/
void free_track(track_t* t);

/*concatène tous les éléments de t.sounds*/
sound_t* reduce_track(track_t* t);
