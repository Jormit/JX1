#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <windows.h>

#define STAGE_ATTACK   0
#define STAGE_DECAY    1
#define STAGE_SUSTAIN  2
#define STAGE_RELEASE  3

typedef struct {
    char code;
    char stage;
    float time;
    float freq;
} note;

// All the notes played at a particular instant. Max 4.
typedef struct {
    note note1;
    note note2;
    note note3;
    note note4;
} notes_pressed;


typedef struct {
    float *envelope_table;
    float attack;
    float decay;
    float sustain;
    float release;
} envelope;


char Key2Note(KEY_EVENT_RECORD ker);
VOID KeyEventProc(KEY_EVENT_RECORD ker, notes_pressed *notes);
VOID initialize_keys(notes_pressed *notes);

typedef struct {
    float phase;
    envelope *envelope;
    float *wavetable;
    float mix;
} osc;

typedef struct {
    osc *osc1;
    osc *osc2;
    notes_pressed *notes;
} osc_pack;

// Oscillator functions.
osc *create_new_osc(float *wavetable, float mix);
void add_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq, note *current_note);
void clear_osc(float *output, unsigned long frameCount);

envelope *create_envelope(float attack, float decay, float sustain, float release, int sample_rate);

#endif // OSC_H_INCLUDED
