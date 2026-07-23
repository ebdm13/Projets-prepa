#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "sound.h"
#include "wav.h"
#include "melody.h"
#include<stdlib.h>
#include<time.h>

int main(int argc, char** argv){
	assert(argc == 3);
	int f_ech = 44100;
	// FILE* f = fopen(argv[1], "r");
	// track_t* t = read_track(f);
	// sound_t* s = reduce_track(t);
	// save_sound(argv[2],s);
	// free_sound(s);
	// free_track(t);
	// fclose(f);
	// test();
	double t = time(NULL);
	mix_t* m = load_mix(argv[1]);
	sound_t* s = reduce_mix(m);
	save_sound(argv[2], s);
	printf("Fichier %s généré (temps écoulé: %.3fs)\n", argv[2], time(NULL) - t);
	int size = s->n_samples * 2 / 1000000;
	float duree = (float)(s->n_samples) / f_ech;
	printf("Durée du fichier: %dm%ds (taille: %dMo)\n", (int)(duree / 60), (int)(duree) % 60, size);
	free_mix(m);
	free_sound(s);
}
