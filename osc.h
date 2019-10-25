#ifndef OSC_H_INCLUDED
#define OSC_H_INCLUDED

#include <malloc.h>

typedef struct {
    float left_phase;
    float right_phase;
    float *wavetable;
} osc;

osc *create_new_osc(float *wavetable);
void render_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq);

#endif // OSC_H_INCLUDED
