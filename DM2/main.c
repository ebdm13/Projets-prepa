#include<stdio.h>
#include<stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "sound.c"

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

int main(){
	int16_t val[3] = {15387, 815, -6337};
	sound_t court = {
		.n_samples = 3,
		.samples = val,
	};
	save_sound("test2.wav", &court);

}