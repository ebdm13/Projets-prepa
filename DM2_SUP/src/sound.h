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

typedef struct {
	int n_track;
	track_t** tracks;
	double* vols;
} mix_t;

/*Libère la mémoire alouée pour s*/
void free_sound(sound_t* s);

/*Libère la mémoire alouée pour t*/
void free_track(track_t* t);

/*Libère la mémoire alouée pour m*/
void free_mix(mix_t* m);

/*concatène tous les éléments de t.sounds*/
sound_t* reduce_track(track_t* t);

/*Transforme m en un son en prenant la moyenne pondérée des valeurs pour chaque échantillon*/
sound_t* reduce_mix(mix_t* m);
