#include "sound.h"
#include <stdlib.h>

void free_sound(sound_t* s){
	free(s->samples);
	free(s);
}
