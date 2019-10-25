#include "osc.h"

osc *create_new_osc(float *wavetable) {
    osc *new_osc = malloc(sizeof(osc));
    new_osc->left_phase = 0;
    new_osc->right_phase = 0;
    new_osc->wavetable = wavetable;
    return new_osc;
}

void render_osc(float *buffer, osc *osc, unsigned long frameCount, int sample_rate, float freq) {
    unsigned int i;
    for (i=0; i < frameCount; i++){
        *buffer++ = osc->wavetable[(int)(osc->left_phase * sample_rate)];
        *buffer++ = osc->wavetable[(int)(osc->right_phase * sample_rate)];

        osc->left_phase += ( freq / sample_rate);
        osc->right_phase += ( freq / sample_rate);

        if( osc->left_phase >= 1.0f ) osc->left_phase = 0.0f;
        if( osc->right_phase >= 1.0f ) osc->right_phase = 0.0f;
    }
}
