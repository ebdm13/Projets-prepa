#pragma once
#include "sound.h"

// Strucure pour les différent types de signaux. Ex: sawtooth, triangle ...
typedef sound_t* (*signal_t)(float frequence, float amplitude, float duree, int f_ech);
typedef struct {
	char* nom;
	signal_t signale;	
} signaux_t;

/*renvoi le signal corresondant au nom*/
signal_t get_signal(char* nom);

/*renvoie les echantillons d'un bruits blanc échantilloné avec une fréquence f_ech prendant une durée duree en secondes*/
sound_t* white(float duree, int f_ech);

/*renvoie les echantillons d'un signale sinusoïdal correspondant à la frequence, amplitude, duree donnée*/
sound_t* sine(float frequence, float amplitude, float duree, int f_ech);

/*renvoie les echantillons d'un signale carré correspondant à la frequence, amplitude, duree donnée*/
sound_t* square(float frequence, float amplitude, float duree, int f_ech);

/*renvoie les echantillons d'un signale triangle correspondant à la frequence, amplitude, duree donnée*/
sound_t* triangle(float frequence, float amplitude, float duree, int f_ech);

/*renvoie les echantillons d'un signale en dent de scie correspondant à la frequence, amplitude, duree donnée*/
sound_t* sawtooth(float frequence, float amplitude, float duree, int f_ech);
