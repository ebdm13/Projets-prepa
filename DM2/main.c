#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "sound.h"
#include "waveform.h"
#include "melody.h"
#include<stdlib.h>
#include<time.h>

int f_ech = 44100;
int L = 16;
int nb_canaux = 1;

/*Ecrit l'entier a sur un nombre de "size" octets dans f en petit boutiste*/
void write_int(FILE* f, int a, int size){
	assert(f != NULL);
	// assert(a < 1 << (8 * size));
	for (int i = 0; i < size;i++){
		fprintf(f, "%c", a  % 256);
		a = a >> 8;
	}
}


/*Ecrit le header du fichier wave dans f. où n est le nombre d'échantillons*/
void write_header(FILE* f, int n){
	fprintf(f, "RIFF");
	write_int(f, 36 + n * L / 8, 4);
	fprintf(f, "WAVEfmt %c%c%c%c%c%c", 0x10, 0x00, 0x00, 0x00, 0x01, 0x00);
	write_int(f, nb_canaux, 2);
	write_int(f, f_ech, 4);
	write_int(f, f_ech * L / 8, 4);
	write_int(f, nb_canaux * L / 8, 2);
	write_int(f, L, 2);
	fprintf(f, "data");
	write_int(f, n * L / 8, 4);
}

/*sauvegarde les échantillons de s dans filename, un fichier .wav*/
void save_sound(char* filename, sound_t* s){
	FILE* f = fopen(filename, "w");
	assert(f != NULL);
	write_header(f, s->n_samples);
	for (int i = 0; i < s->n_samples; i++){
		write_int(f, s->samples[i], L/8);
	}
	fclose(f);
}
void test(){
	int16_t val[3] = {15387, 815, -6337};
	sound_t court = {
		.n_samples = 3,
		.samples = val,
	};
	save_sound("test2.wav", &court);
	srand(time(NULL));
	sound_t* s = white(10, f_ech);
	save_sound("bruit.wav", s);
	free_sound(s);
	s = sine(440, 10000, 10, f_ech);
	save_sound("440.wav", s);
	free_sound(s);
	s = square(440, 10000, 10, f_ech);
	save_sound("square.wav", s);
	free_sound(s);
	s = triangle(440, 10000, 10, f_ech);
	save_sound("triangle.wav", s);
	free_sound(s);
	s = sawtooth(440, 10000, 10, f_ech);
	save_sound("sawtooth.wav", s);
	free_sound(s);
	track_t* t = malloc(sizeof(track_t));
	t->sounds = malloc(9 * sizeof(sound_t*));
	t->sounds[0] = sine(440, 0, 0.5, f_ech);
	t->sounds[1] = sine(440, 16000, 0.4, f_ech);
	t->sounds[2] = sine(440, 0, 0.1, f_ech);
	t->sounds[3] = sine(440, 16000, 0.4, f_ech);
	t->sounds[4] = sine(440, 0, 0.1, f_ech);
	t->sounds[5] = sine(440, 16000, 0.5, f_ech);
	t->sounds[6] = sine(493.88, 16000, 0.5, f_ech);
	t->sounds[7] = sine(554.36, 16000, 1, f_ech);
	t->sounds[8] = sine(493.88, 16000, 1, f_ech);
	t->n_sounds = 9;
	s = reduce_track(t);
	save_sound("Au clair de la lune.wav", s);
	free_track(t);
	free_sound(s);

}
int main(){
	// test();
	FILE* f = fopen("../archive/question_15/sonata_une_piste.txt", "r");
	track_t* t = read_track(f);
	sound_t* s = reduce_track(t);
	save_sound("sonata.wav",s);
	free_sound(s);
	free_track(t);
	fclose(f);
	}