#include "sound.h"
#include <stdlib.h>

void free_sound(sound_t* s){
	free(s->samples);
	free(s);
}

void free_track(track_t *t){
	for (int i = 0; i < t->n_sounds; i++){
		free_sound(t->sounds[i]);
	}
	free(t->sounds);
	free(t);
}

sound_t* reduce_track(track_t *t){
	sound_t* s = malloc(sizeof(sound_t));
	s->n_samples = 0;
	for (int i = 0; i < t->n_sounds; i++){
		s->n_samples += t->sounds[i]->n_samples;
	}
	int16_t* samples = malloc(s->n_samples * sizeof(int16_t));
	int n = 0;
	for (int i = 0; i < t->n_sounds; i++){
		for (int j = 0; j < t->sounds[i]->n_samples; j++){
			samples[n] = t->sounds[i]->samples[j];
			n++;
		}
	}
	s->samples = samples;
	return s;
}
