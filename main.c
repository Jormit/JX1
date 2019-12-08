#include <stdio.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "include/portaudio.h"

// User synth defines.
#include "wavetables.h"
#include "osc.h"
#include "filters.h"

#define SAMPLE_RATE (44100)

// Define of the audio callback function.
static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

// Variables to handle keypresses.
HANDLE hStdin;
DWORD fdwSaveOldMode;

// These are needed to allow filter to work continously to remove artefacts.
float last_in[2];
float last_out[2];

int main(void){
    // Initialize keypress api.
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    last_in[0] = 0.0;
    last_in[1] = 0.0;
    last_out[0] = 0.0;
    last_out[1] = 0.0;

    // Initialize struct to hold notes.
    notes_pressed *notes = malloc(sizeof(notes_pressed));
    initialize_keys(notes);

    printf("Sample rate %d\n", SAMPLE_RATE);

    // Initiate wave tables.
    float *sqr_table = create_wavetable(TYPE_SQUARE, SAMPLE_RATE, 128);
    float *saw_table = create_wavetable(TYPE_SAW, SAMPLE_RATE, 128);
    float *sin_table = create_wavetable(TYPE_SINE, SAMPLE_RATE, 128);

    // Initiate oscilators.
    osc *osc1 = create_new_osc(sqr_table, 0.5);
    osc *osc2 = create_new_osc(saw_table, 0.5);

    // Create_envelope (release doesn't really do anything atm).
    envelope *env1 = create_envelope(0.05, 0.4, 0.5, 0.3, SAMPLE_RATE);
    osc1->envelope = env1;
    osc2->envelope = env1;

    // Pack oscilators for transfer to callback function.
    osc_pack *oscillators = malloc(sizeof(osc_pack));
    oscillators->osc1 = osc1;
    oscillators->osc2 = osc2;
    oscillators->notes = notes;

    // Start audio stream callback.
    PaStream *stream;
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 1, paFloat32, SAMPLE_RATE, 256, pa_callback, oscillators);
    Pa_StartStream( stream );

    // While running listen for keypresses.
    while (1) {
        // Wait for the events.
        ReadConsoleInput( hStdin, irInBuf, 128, &cNumRead);
        for (i = 0; i < cNumRead; i++) {
            if (irInBuf[i].EventType ==  KEY_EVENT) {
                KeyEventProc(irInBuf[i].Event.KeyEvent, notes);
            }
        }
    }

    Pa_StopStream( stream );
    Pa_CloseStream( stream );

    free(osc1); free(osc2); free(oscillators); free(sqr_table); free(saw_table); free(sin_table);

    return 0;
}

/** Function called when audio buffer needs more data **/
static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ) {

    osc_pack *data = (osc_pack*)userData;

    clear_osc(output, frameCount);
    float env_amp = data->osc1->envelope->sustain;

    if (data->notes->note1.code) {
        add_osc(output, data->osc1, frameCount, SAMPLE_RATE, data->notes->note1.freq/4.0, &data->notes->note1);
        add_osc(output, data->osc2, frameCount, SAMPLE_RATE, data->notes->note1.freq/4.0, &data->notes->note1);
        if (data->notes->note1.time < data->osc1->envelope->attack + data->osc1->envelope->decay){
            env_amp = data->osc1->envelope->envelope_table[(int)(data->notes->note1.time * SAMPLE_RATE)];
        } else {

        }
    }

    filter_coeff coeff = calculate_coefficients (env_amp*env_amp * 2000, SAMPLE_RATE, LOW_PASS, sqrt(2));
    filter(output, frameCount, coeff, last_in, last_out);

    /**
    if (oscillators->notes->note2.code) {
        add_osc(output, oscillators->osc1, frameCount, SAMPLE_RATE, oscillators->notes->note2.freq);
        add_osc(output, oscillators->osc2, frameCount, SAMPLE_RATE, oscillators->notes->note2.freq);
    } if (oscillators->notes->note3.code) {
        add_osc(output, oscillators->osc1, frameCount, SAMPLE_RATE, oscillators->notes->note3.freq);
        add_osc(output, oscillators->osc2, frameCount, SAMPLE_RATE, oscillators->notes->note3.freq);
    } if (oscillators->notes->note4.code) {
        add_osc(output, oscillators->osc1, frameCount, SAMPLE_RATE, oscillators->notes->note4.freq);
        add_osc(output, oscillators->osc2, frameCount, SAMPLE_RATE, oscillators->notes->note4.freq);
    } **/

    return 0;
}
