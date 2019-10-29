#include "osc.h"
#include <malloc.h>

osc *create_new_osc(float *wavetable, float mix) {
    osc *new_osc = malloc(sizeof(osc));
    new_osc->left_phase = 0;
    new_osc->right_phase = 0;
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

        *buffer++ += env_amp * osc->mix * osc->wavetable[(int)(osc->left_phase * sample_rate)];
        *buffer++ += env_amp * osc->mix * osc->wavetable[(int)(osc->right_phase * sample_rate)];

        osc->left_phase += ( freq / sample_rate);
        osc->right_phase += ( freq / sample_rate);

        if( osc->left_phase >= 1.0f ) osc->left_phase = 0.0f;
        if( osc->right_phase >= 1.0f ) osc->right_phase = 0.0f;
    }
}

void clear_osc(float *buffer, unsigned long frameCount) {
    int i;
    for (i=0; i < frameCount; i++){
        *buffer++ = 0.0;
        *buffer++ = 0.0;
    }
}
