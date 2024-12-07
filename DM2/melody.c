#include <math.h>
#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include "melody.h"
#include "sound.h"
#include "waveform.h"


float pitch_to_freq(float pitch){
	return 440 * pow(2, pitch / 12);
}

track_t* read_track(FILE* file){
	assert(file != NULL);
	int n;
	char* nom_signal = malloc(15 * sizeof(char));
	fscanf(file, "%d %s", &n, nom_signal);
	signal_t signal = get_signal(nom_signal);
	assert(signal != NULL);
	track_t* t = malloc(sizeof(track_t));
	t->n_sounds = n;
	t->sounds = malloc(n * sizeof(sound_t*));
	int i = 0;
	int pitch;
	float frequence;
	float amplitude;
	float duree;
	float volume;
	while (i < n && fscanf(file, "%d %f %f", &pitch, &duree, &volume) != EOF){
		frequence = pitch_to_freq(pitch);
		amplitude = 32676 * volume;
		t->sounds[i] = signal(frequence, amplitude, duree, 44100);
		i++;
	}
	assert(i == n);
	free(nom_signal);
	return t;
}
