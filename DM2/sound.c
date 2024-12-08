#include "sound.h"
#include <assert.h>
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

void free_mix(mix_t* m){
	for (int i = 0; i < m->n_track; i++){
		free_track(m->tracks[i]);
	}
	free(m->tracks);
	free(m->vols);
	free(m);
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

sound_t* reduce_mix(mix_t* m){
	assert((m != NULL) & (m->n_track != 0) && (m->tracks != NULL) & (m->vols != NULL));
	sound_t* s = malloc(sizeof(sound_t));
	sound_t* s_temp;
	int n = m->tracks[0]->sounds[0]->n_samples;
	int* samples_sum = calloc(n, sizeof(int));

	for (int i = 0; i < m->n_track; i++){
		s_temp = reduce_track(m->tracks[i]);
		if (s_temp->n_samples > n){
			samples_sum = realloc(samples_sum, s_temp->n_samples * sizeof(int));
			for (int k = n; k < s_temp->n_samples; k++){
				samples_sum[k] = 0;	
			}
			n = s_temp->n_samples;
		}
		for (int j = 0; j < s_temp->n_samples;j++){
			samples_sum[j] += s_temp->samples[j] * m->vols[i];
		}
		free_sound(s_temp);
	}
	for (int p = 0; p < n; p++){
		samples_sum[p] /= m->n_track;
	}
	s->n_samples = n;
	s->samples = malloc(n * sizeof(int16_t));
	for (int i = 0; i < n; i++){
		s->samples[i] = (int16_t)samples_sum[i];
	}
	free(samples_sum);
	return s;
}
