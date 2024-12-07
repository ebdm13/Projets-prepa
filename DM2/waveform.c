#include "waveform.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

signaux_t signaux[] = {
	{"sine", sine},
	{"square", square},
	{"triangle", triangle},
	{"sawtooth", sawtooth},
	{NULL, NULL}
};

signal_t get_signal(char *nom){
	for (signaux_t *sg = signaux; sg->nom != NULL; sg++){
		if (strcmp(nom, sg->nom) == 0){
			return sg->signale; 
		}	
	}
	return NULL;
}

sound_t* white(float duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int16_t* samples = malloc(duree * f_ech * sizeof(int16_t));
	for (int i = 0; i < duree * f_ech; i++){
		samples[i] = (rand() % 65536) - 32768;
	}
	s->samples = samples;
	s->n_samples = duree * f_ech;
	return s;
}

sound_t* sine(float frequence, float amplitude, float duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int16_t* samples = malloc(duree * f_ech * sizeof(int16_t));
	for (int i = 0; i < duree * f_ech; i++){
		samples[i] = amplitude * sin(2 * M_PI * frequence * i / f_ech);
	}
	s->samples = samples;
	s->n_samples = duree * f_ech;
	return s;
}

sound_t* square(float frequence, float amplitude, float duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int16_t* samples = malloc(duree * f_ech * sizeof(int16_t));
	float f_r = frequence / f_ech;
	int signe = 1;
	for (int i = 0; i < duree * f_ech; i++){
		if (i % (int)(1/(2 * f_r)) == 0){
			signe *= -1;
		}
		samples[i] = amplitude * signe; 
	}
	s->samples = samples;
	s->n_samples = duree * f_ech;
	return s;
}

sound_t* triangle(float frequence, float amplitude, float duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int16_t* samples = malloc(duree * f_ech * sizeof(int16_t));
	float f_r = frequence / f_ech;
	float pente = 4 * amplitude * f_r;
	int signe = -1;
	float origine = 0; 
	for (int i = 0; i < duree * f_ech; i++){
		if (i % (int)(1 / (2 * f_r)) == 0) {
			signe *= -1;
			origine = i; 
		}
		samples[i] = (i - origine) * signe * pente - amplitude * signe;
	}
	s->samples = samples;
	s->n_samples = duree * f_ech;
	return s;
}

sound_t* sawtooth(float frequence, float amplitude, float duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int16_t* samples = malloc(duree * f_ech * sizeof(int16_t));
	float f_r = frequence / f_ech;
	float pente = 2 * amplitude * f_r;
	float origine = 0; 
	for (int i = 0; i < duree * f_ech; i++){
		if (i % (int)(1 / f_r) == 0) {
			origine = i; 
		}
		samples[i] = (i - origine) * pente - amplitude;
	}
	s->samples = samples;
	s->n_samples = duree * f_ech;
	return s;
}