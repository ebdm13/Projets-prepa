#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../renderer.h"
#include "../AST.h"
#include "../token.h"
#include "../error.h"
#include "../sound.h"

void test_create_note() {
    printf("[Test] create_note:\n");

    note_t* note = create_note(3, 16);
    assert(note != NULL);
    assert(note->pitch == 3);
    assert(note->duree == 16);

    free(note);
    info(0,"\t[✓] create_note a passé le test.\n");
}

void test_duree_to_second() {
    printf("[Test] duree_to_second:\n");

    float seconds = duree_to_second(4, 4, 120);
    assert(seconds == 0.5f);

    seconds = duree_to_second(16, 4, 60);
    assert(seconds == 0.25f);

    info(0, "\t[✓] duree_to_second a passé le test.\n");
}

void test_render_note() {
    printf("[Test] render_note:\n");

    // Préparation d'un noeud AST pour une note
    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(NOTE, "c'4");
    queue->linenumber = 1;
    queue->next = NULL;
    add_token(queue, create_token(NOTE_NAME, "c"), 1);
    add_token(queue, create_token(OCTAVE, "'"), 1);
    add_token(queue, create_token(DUREE, "4"), 1);
    AST_node* note_node = create_node(queue);
    add_child(note_node, create_node(queue->next));
    add_child(note_node, create_node(queue->next->next));
    add_child(note_node, create_node(queue->next->next->next));

    note_t* note = render_note(note_node);
    assert(note->pitch == -9 + 12); // 'c' + octave up
    assert(note->duree == 4);

    free(note);
    free_list(queue);
    free_AST(note_node);
    info(0, "\t[✓] render_note a passé le test.\n");
}

void test_render() {
    printf("[Test] render:\n");

    TokenNode* queue = malloc(sizeof(TokenNode));
    queue->token = create_token(BOT, "");
    queue->linenumber = 0;
    queue->next = NULL;
    add_token(queue, create_token(NOTE, "c'4"), 1);
    add_token(queue, create_token(NOTE_NAME, "c"), 1);
    add_token(queue, create_token(OCTAVE, "'"), 1);
    add_token(queue, create_token(DUREE, "4"), 1);
    add_token(queue, create_token(REST, "r4"), 1);

    AST_node* root = create_node(queue);
    AST_node* note1 = create_node(queue->next);
    add_child(note1, create_node(queue->next->next));
    add_child(note1, create_node(queue->next->next->next));
    add_child(note1, create_node(queue->next->next->next->next));

    AST_node* rest1 = create_node(queue->next->next->next->next->next);

    add_child(root, note1);
    add_child(root, rest1);

    int f_ech = 44100;
    int duree_ref = 1;
    int bpm = 120;

    track_t* track = render(root, f_ech, duree_ref, bpm);

    assert(track != NULL);
    assert(track->n_sounds == 2);
    assert(track->sounds[0] != NULL);
    assert(track->sounds[1] != NULL);

    free_track(track);
    free_list(queue);
    free_AST(root);
    info(0, "\t[✓] render a passé le test.\n");
}

void test_renderer() {
    test_create_note();
    test_duree_to_second();
    test_render_note();
    test_render();

    printf("\nTous les tests de renderer.c sont terminés avec succès.\n");
}
