#include "waveform.h"
#include <stdlib.h>
#include <math.h>

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