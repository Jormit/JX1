#include "wavetables.h"
#include <stdio.h>

float *create_wavetable(int type, int sample_rate, int harmonics){
    /** A plain sine wave **/
    if (type == TYPE_SINE){
        float *new_table = malloc(sizeof(float) * sample_rate);

        int i;
        for (i = 0; i < sample_rate; i++){
            new_table[i] = sin((2.0 * M_PI * i)/(float)sample_rate);
        }
        return new_table;

    /** Saw wave consists of all integer multiple sine waves with reciprocal amplitude. **/
    } else if (type == TYPE_SAW) {
        float *new_table = malloc(sizeof(float) * sample_rate);

        int i, harmonic_count;
        for (i = 0; i < sample_rate; i++){
            for (harmonic_count = 1; harmonic_count <= harmonics; harmonic_count++){
                new_table[i] += (2/M_PI) * (1.0 / (float)harmonic_count) *
                sin((2.0 * M_PI * i * harmonic_count)/(float)sample_rate);
            }
        }
        return new_table;

    /** Square wave consists of all odd integer multiple sine waves with reciprocal amplitude. **/
    } else if (type == TYPE_SQUARE) {
        float *new_table = malloc(sizeof(float) * sample_rate);

        int i, harmonic_count;
        for (i = 0; i < sample_rate; i++){
            for (harmonic_count = 1; harmonic_count <= harmonics; harmonic_count+=2){
                new_table[i] += (4/M_PI) * (1.0 / (float)harmonic_count) *
                sin((2.0 * M_PI * i * harmonic_count)/(float)sample_rate);
            }
        }
        return new_table;
    }

    else {
        return NULL;
    }
}
