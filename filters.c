#include "filters.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

/** Calculate the filter coefficients
 *  Works for lowpass and highpass with different cutoffs and resonances (change with q).
 *  See: http://creatingsound.com/2014/02/dsp-audio-programming-series-part-2/.
 */
filter_coeff calculate_coefficients (int cutoff_freq, int sample_rate, int type, float q) {
    float theta = 2. * M_PI * ((float)cutoff_freq / (float)sample_rate);
    float d = 0.5 * (1. / q) * sin(theta);
    float beta = 0.5 * ( (1. - d) / (1. + d) );
    float gamma = (0.5 + beta) * cos(theta);

    filter_coeff out_coeff;

    if (type == LOW_PASS) {
        out_coeff.a0 = 0.5 * (0.5 + beta - gamma);
        out_coeff.a1 = 0.5 + beta - gamma;
    }
    else {
        out_coeff.a0 = 0.5 * (0.5 + beta + gamma);
        out_coeff.a1 = -(0.5 + beta + gamma);
    }

    out_coeff.a2 = out_coeff.a0;
    out_coeff.b1 = -2. * gamma;
    out_coeff.b2 = 2. * beta;

    return out_coeff;
}

/** Applies filter to a block of audio.
 *  This uses a basic iir filter with the equation:
 *  y(n) = a0x(n) + a1x(n-1) + a2x(n-2) – b1y(n-1) – b2y(n-2)
 *  Where a* and b* are the filter coefficients calculated by calculate_coefficients().
 *  Note that we must save the last two inputs/outputs of each block to eliminate
 *  edge effects (Causes cracking). *
 *  See: http://creatingsound.com/2014/02/dsp-audio-programming-series-part-2/.
 */
void filter(float *out, int buffer_size, filter_coeff coefficients, float *last_in, float *last_out) {
    float in[buffer_size];
    double output;
    int i;
    for (i = 0; i < buffer_size; i++){
        in[i] = out[i];
    }

    // Apply previous values.
    if (last_out[0]){
        output =  ((coefficients.a0 * in[0]) + (coefficients.a1 * last_in[1]) + (coefficients.a2 * last_in[0])
         - (coefficients.b1 * last_out[1]) - (coefficients.b2 * last_out[0]));
        out[0] = (float)output;

        output =  ((coefficients.a0 * in[1]) + (coefficients.a1 * in[0]) + (coefficients.a2 * last_in[1])
         - (coefficients.b1 * out[0]) - (coefficients.b2 * last_out[1]));
        out[1] = (float)output;
    }

    // Apply to rest of values in block.
    for (i = 2; i < buffer_size; i++){
        output =  ((coefficients.a0 * in[i]) + (coefficients.a1 * in[i-1]) + (coefficients.a2 * in[i-2])
         - (coefficients.b1 * out[i-1]) - (coefficients.b2 * out[i-2]));

        out[i] = (float)output;
    }
    i--;

    // Save previous buffer to eliminate artefacts.
    last_in[0] = (float)in[i-1];
    last_in[1] = (float)in[i];
    last_out[0] =(float)out[i-1];
    last_out[1] =(float)out[i];

    return;
}
