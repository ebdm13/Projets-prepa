#pragma once
#include "sound.h"

/*renvoie les echantillons d'un bruits blanc échantilloné avec une fréquence f_ech prendant une durée duree en secondes*/
sound_t* white(float duree, int f_ech);

/*renvoie les echantillons d'un signale sinusoïdal correspondant à la frequence, amplitude, duree donnée*/
sound_t* sine(float frequence, float amplitude, float duree, int f_ech);