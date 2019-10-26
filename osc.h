#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <malloc.h>

typedef struct {
    float left_phase;
    float right_phase;
    float *wavetable;
    float mix;
} osc;

typedef struct {
    osc *osc1;
    osc *osc2;
} osc_pack;


osc *create_new_osc(float *wavetable, float mix);
void render_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq);
void add_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq);
void mix_osc(float *buffer, osc *osc1, osc *osc2, unsigned long frameCount, int sample_rate, float freq);

#endif // OSC_H_INCLUDED
