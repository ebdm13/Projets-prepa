#include <math.h>
#include <assert.h>
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
	assert(n != 0);
	signal_t signal = get_signal(nom_signal);
	assert(signal != NULL);
	track_t* t = malloc(sizeof(track_t));
	t->n_sounds = n;
	t->sounds = malloc(n * sizeof(sound_t*));
	int i = 0;
	float pitch;
	double frequence;
	double amplitude;
	double duree;
	double volume;
	while (i < n && fscanf(file, "%f %lf %lf", &pitch, &duree, &volume) != EOF){
		frequence = pitch_to_freq(pitch);
		amplitude = 32676 * volume;
		t->sounds[i] = signal(frequence, amplitude, duree, 44100);
		i++;
	}
	assert(i == n);
	free(nom_signal);
	return t;
}

mix_t* load_mix(char *filename){
	FILE* f = fopen(filename, "r");
	assert(f != NULL);
	mix_t* m = malloc(sizeof(mix_t));
	int a = fscanf(f, "%d", &(m->n_track));
	m->vols = malloc(m->n_track * sizeof(double));
	m->tracks = malloc(m->n_track * sizeof(track_t*));
	for (int i = 0;i<m->n_track && a != EOF;i++){
		a = fscanf(f, "%lf", &(m->vols[i]));
	}
	for (int i = 0;i<m->n_track;i++){
		m->tracks[i] = read_track(f);
	}
	fclose(f);
	return m;
}
