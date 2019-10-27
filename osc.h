#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <malloc.h>
#include <windows.h>

typedef struct {
    char code;
    float freq;
} note;

// All the notes played at a particular instant. Max 4.
typedef struct {
    note note1;
    note note2;
    note note3;
    note note4;
} notes_pressed;

char Key2Note(KEY_EVENT_RECORD ker);
VOID KeyEventProc(KEY_EVENT_RECORD ker, notes_pressed *notes);
VOID initialize_keys(notes_pressed *notes);

typedef struct {
    float left_phase;
    float right_phase;
    float *wavetable;
    float mix;
} osc;

typedef struct {
    osc *osc1;
    osc *osc2;
    notes_pressed *notes;
} osc_pack;


osc *create_new_osc(float *wavetable, float mix);
void render_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq);
void add_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq);
void clear_osc(float *output, unsigned long frameCount);

#endif // OSC_H_INCLUDED
