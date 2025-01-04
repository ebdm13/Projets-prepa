#include "renderer.h"
#include "lexer.h"
#include "parser.h"
#include "wav.h"
#include "error.h"
#include <assert.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv){
	assert(argc >= 3);
	FILE* f = fopen(argv[1], "r");
	clock_t start, end;
    double cpu_time_used;

    clock_t start1 = clock();
    TokenNode* tokenQueue = lexer(f);
    end = clock();
    cpu_time_used = ((double) (end - start1)) / CLOCKS_PER_SEC;
    info(0, "[✓] Lexer: %f seconds\n", cpu_time_used);
	fclose(f);
	start = clock();
	AST_node* root = parse(tokenQueue);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    info(0, "[✓] Parser: %f seconds\n", cpu_time_used);
	start = clock();
	track_t* track = render(root, 44100, 4, 120);
	sound_t* sound = reduce_track(track);
	save_sound(argv[2], sound);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    info(0, "[✓] Render: %f seconds\n", cpu_time_used);
    cpu_time_used = ((double) (end - start1)) / CLOCKS_PER_SEC;
    info(0, "Total: %f seconds\n", cpu_time_used);
    if (argc == 4){
    	printf("\n");
    	if (strcmp(argv[3], "-p") == 0) {
    		print_AST(root);
    	} else if (strcmp(argv[3], "-l") == 0) {
    		print_tokens(tokenQueue);
    	} else if (strcmp(argv[3], "-lp") == 0 || strcmp(argv[3], "-pl") == 0){
    		print_tokens(tokenQueue);
    		printf("\n");
    		print_AST(root);
    	} else {
    		error(0, "Argument incorrecte: %s\n", argv[3]);
    	}
    }
	free_list(tokenQueue);
	free_AST(root);
	free_track(track);
	free_sound(sound);
	return 0;	
}