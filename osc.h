#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <malloc.h>

typedef struct {
    float left_phase;
    float right_phase;
    float curr_freq;
    float *wavetable;
} osc;

osc *create_new_osc(float *wavetable, float freq);
void render_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate);

#endif // OSC_H_INCLUDED
