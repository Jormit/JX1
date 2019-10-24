#include "wavetables.h"
#include <stdio.h>

float *create_wavetable(int type, int sample_rate){
    if (type == TYPE_SINE){
        float *new_table = malloc(sizeof(float) * sample_rate);

        int i;
        for (i = 0; i < sample_rate; i++){
            new_table[i] = sin((2.0 * M_PI * i)/(float)sample_rate);
        }
        return new_table;
    } else {
        return NULL;
    }
}
