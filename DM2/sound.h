#pragma once
#include <stdint.h>
#include <stdio.h>

typedef struct {
	int n_samples;
	int16_t* samples;
} sound_t;

/*Libère la mémoire alouée pour s*/
void free_sound(sound_t* s);
