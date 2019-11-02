#include "osc.h"
#include <malloc.h>

osc *create_new_osc(float *wavetable, float mix) {
    osc *new_osc = malloc(sizeof(osc));
    new_osc->phase = 0;
    new_osc->wavetable = wavetable;
    new_osc->mix = mix;
    return new_osc;
}

void add_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq, note *current_note) {
    unsigned int i;
    float env_amp = osc->envelope->sustain;

    for (i=0; i < frameCount; i++){
        current_note->time+=0.00002267573;
        if (current_note->time < osc->envelope->attack + osc->envelope->decay){
            env_amp = osc->envelope->envelope_table[(int)(current_note->time * sample_rate)];
        }

        *buffer++ += 0.5 * env_amp * osc->mix * osc->wavetable[(int)(osc->phase * sample_rate)];
        osc->phase += ( freq / sample_rate);
        if( osc->phase >= 1.0f ) osc->phase = 0.0f;

    }
}

void clear_osc(float *buffer, unsigned long frameCount) {
    int i;
    for (i=0; i < frameCount; i++){
        *buffer++ = 0.0;
    }
}
