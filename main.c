#include <stdio.h>
#include <math.h>
#include <string.h>
#include "include/portaudio.h"

// Synth defines.
#include "wavetables.h"

#include "osc.h"

#define SAMPLE_RATE (44100)

static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

// Variables to handle keypresses.
HANDLE hStdin;
DWORD fdwSaveOldMode;

int main(void){
    // Initialize keypress api.
    DWORD cNumRead, i;
    INPUT_RECORD irInBuf[128];
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    // Initialize struct to hold notes.
    notes_pressed *notes = malloc(sizeof(notes_pressed));
    initialize_keys(notes);

    // Initiate wave tables.
    float *sqr_table = create_wavetable(TYPE_SQUARE, SAMPLE_RATE, 64);
    float *saw_table = create_wavetable(TYPE_SAW, SAMPLE_RATE, 64);
    float *sin_table = create_wavetable(TYPE_SINE, SAMPLE_RATE, 64);

    // Initiate oscilators.
    osc *osc1 = create_new_osc(sqr_table, 0.125);
    osc *osc2 = create_new_osc(saw_table, 0.125);

    // Pack oscilators for transfer to callback function.
    osc_pack *oscillators = malloc(sizeof(osc_pack));
    oscillators->osc1 = osc1;
    oscillators->osc2 = osc2;
    oscillators->notes = notes;

    // Start audio stream callback.
    PaStream *stream;
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, pa_callback, oscillators);
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

    osc_pack *oscillators = (osc_pack*)userData;

    clear_osc(output, frameCount);

    if (oscillators->notes->note1.code) {
        add_osc(output, oscillators->osc1, frameCount, SAMPLE_RATE, oscillators->notes->note1.freq);
        add_osc(output, oscillators->osc2, frameCount, SAMPLE_RATE, oscillators->notes->note1.freq);
    }
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
