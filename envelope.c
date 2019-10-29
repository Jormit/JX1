#include "osc.h"

envelope *create_envelope(float attack, float decay, float sustain, float release, int sample_rate){
    envelope *new_envelope = malloc(sizeof(envelope));
    float *new_envelope_table = malloc(sizeof(float) * sample_rate * (attack + decay));
    int i = 0;
    int attack_range = (sample_rate * attack);
    int decay_range = (sample_rate * decay);

    // Create a envelope table.
    while (i < attack_range) {
        new_envelope_table[i] = i * (1 / (float)(attack_range));
        i++;
    }
    i = 0;
    while (i < decay_range) {
        new_envelope_table[i  +  attack_range] =  1 + (sustain - 1)/(float)(decay_range) * i;
        i++;
    }

    new_envelope->attack = attack;
    new_envelope->decay = decay;
    new_envelope->sustain = sustain;
    new_envelope->envelope_table = new_envelope_table;

    return new_envelope;
}
