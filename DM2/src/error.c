#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

#define ANSI_RED "\033[31m"
#define ANSI_ORANGE "\033[38;5;208m"
#define ANSI_RESET "\033[0m"

void error(int linenumber, char* format, ...){
	assert(format != NULL);
    va_list args;
    va_start(args, format);

    if (linenumber == 0) {
        fprintf(stderr, ANSI_RED "ERREUR: ");
    } else {
        fprintf(stderr, ANSI_RED "ERREUR (l:%d): ", linenumber);
    }
    vfprintf(stderr, format, args);
    fprintf(stderr, ANSI_RESET "\n");

    va_end(args);
}

void warning(int linenumber, char* format, ...){
	assert(format != NULL);
    va_list args;
    va_start(args, format);
    if (linenumber == 0) {
        fprintf(stderr, ANSI_ORANGE "WARNING: ");
    } else {
        fprintf(stderr, ANSI_ORANGE "WARNING (l:%d): ", linenumber);
    }
    vfprintf(stderr, format, args);
    fprintf(stderr, ANSI_RESET "\n");

    va_end(args);	
}

