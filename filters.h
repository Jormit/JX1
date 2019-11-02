#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#define LOW_PASS  1
#define HIGH_PASS 2

typedef struct {
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
} filter_coeff;

void filter(float *out, int buffer_size, filter_coeff coefficients, float *last_in, float *last_out);
filter_coeff calculate_coefficients (int cutoff_freq, int sample_rate, int type, float q);

#endif // FILTERS_H_INCLUDED
