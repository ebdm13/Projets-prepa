#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void error(char* message, int linenumber){
	assert(message != NULL);
	if (linenumber == 0){
		fprintf(stderr, "\033[31mERREUR: %s\033[0m\n", message);
	} else {
		fprintf(stderr, "\033[31mERREUR (l:%d): %s\033[0m\n", linenumber, message);
	}
	exit(EXIT_FAILURE);
}

void warning(char* message, int linenumber){
	assert(message != NULL);
	if (linenumber == 0){
		fprintf(stderr, "\033[38;5;208mWARNING: %s\033[0m\n", message);
	} else {
		fprintf(stderr, "\033[38;5;208mWARNING (l:%d): %s\033[0m\n", linenumber, message);
	}
}

