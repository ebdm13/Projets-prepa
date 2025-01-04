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
	assert(t != NULL);
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
	assert((m != NULL) && (m->n_track != 0) && (m->tracks != NULL) && (m->vols != NULL));
	sound_t* s_temp;
	int n = 0;
	int max_samples = 0;
	for (int i = 0; i < m->n_track; i++){
		n = 0;
		for (int j = 0; j < m->tracks[i]->n_sounds; j++) {
			n += m->tracks[i]->sounds[j]->n_samples;
		}
		if (n > max_samples){
			max_samples = n;
		}
	}
	int* samples_sum = calloc(max_samples, sizeof(int));
	for (int i = 0; i < m->n_track; i++){
		s_temp = reduce_track(m->tracks[i]);
		for (int j = 0; j < s_temp->n_samples;j++){
			samples_sum[j] += s_temp->samples[j] * m->vols[i];
		}
		free_sound(s_temp);
	}

	sound_t* result = malloc(sizeof(sound_t));
	result->n_samples = max_samples;
	result->samples = malloc(max_samples * sizeof(int16_t));
	for (int i = 0; i < max_samples; i++){
		result->samples[i] = (int16_t)(samples_sum[i] / m->n_track);
	}
	free(samples_sum);
	return result;
}