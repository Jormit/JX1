#ifndef WAVETABLES_H_INCLUDED
#define WAVETABLES_H_INCLUDED

#include <math.h>
#include <malloc.h>

#define TYPE_SINE   1
#define TYPE_SQUARE 2
#define TYPE_SAW    3
// This function will return a wavetable of the specified type.
float *create_wavetable(int type, int sample_rate);

#endif // WAVETABLES_H_INCLUDED
