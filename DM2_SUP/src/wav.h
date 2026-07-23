#pragma once
#include<stdio.h>
#include "sound.h"

/*Ecrit l'entier a sur un nombre de "size" octets dans f en petit boutiste*/
void write_int(FILE* f, int a, int size);

/*Ecrit le header du fichier wave dans f. où n est le nombre d'échantillons*/
void write_header(FILE* f, int n);

/*sauvegarde les échantillons de s dans filename, un fichier .wav*/
void save_sound(char* filename, sound_t* s);