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

sound_t* white(double duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int n = (int)(duree * f_ech)+1;
	s->samples = malloc(n * sizeof(int16_t));
	for (int i = 0; i < n; i++){
		s->samples[i] = (rand() % 65536) - 32768;
	}
	s->n_samples = n;
	return s;
}

sound_t* sine(double frequence, double amplitude, double duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int n = (int)(duree * f_ech)+1;
	s->samples = malloc(n * sizeof(int16_t));
	for (int i = 0; i < n; i++){
		s->samples[i] = (int16_t)(amplitude * sin(2 * M_PI * frequence * i / f_ech));
	}
	s->n_samples = n;
	return s;
}

sound_t* square(double frequence, double amplitude, double duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int n = (int)(duree * f_ech)+1;
	s->samples = malloc(n * sizeof(int16_t));
	double f_r = frequence / f_ech;
	int signe = 1;
	for (int i = 0; i < n; i++){
		if (i % (int)(1/(2 * f_r)) == 0){
			signe *= -1;
		}
		s->samples[i] = (int16_t)(amplitude * signe); 
	}
	s->n_samples = n;
	return s;
}

sound_t* triangle(double frequence, double amplitude, double duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int n = (int)(duree * f_ech)+1;
	s->samples = malloc(n * sizeof(int16_t));
	double f_r = frequence / f_ech;
	double pente = 4 * amplitude * f_r;
	int signe = -1;
	double origine = 0; 
	for (int i = 0; i < n; i++){
		if (i % (int)(1 / (2 * f_r)) == 0) {
			signe *= -1;
			origine = i; 
		}
		s->samples[i] = (int16_t)((i - origine) * signe * pente - amplitude * signe);
	}
	s->n_samples = n;
	return s;
}

sound_t* sawtooth(double frequence, double amplitude, double duree, int f_ech){
	sound_t* s = malloc(sizeof(sound_t));
	int n = (int)(duree * f_ech)+1;
	s->samples = malloc(n * sizeof(int16_t));
	double f_r = frequence / f_ech;
	double pente = 2 * amplitude * f_r;
	double origine = 0; 
	for (int i = 0; i < n; i++){
		if (i % (int)(1 / f_r) == 0) {
			origine = i; 
		}
		s->samples[i] = (int16_t)((i - origine) * pente - amplitude);
	}
	s->n_samples = n;
	return s;
}