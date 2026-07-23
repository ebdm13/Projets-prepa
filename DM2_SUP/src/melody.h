#pragma once
#include "sound.h"

/*Renvoie la fréquence correspondant au pitch*/
float pitch_to_freq(float pitch);

/*Lis un fichier au format:
première ligne : le nombre de notes n puis le type de son à utiliser ;
n lignes suivantes : la hauteur, la durée et le volume de chaque note.
Et renvoie la piste correspondant.
*/
track_t* read_track(FILE* file);

/*Charge depuis un fichier le mix correspondant*/
mix_t* load_mix(char* filename);