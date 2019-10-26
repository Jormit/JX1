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

int main(void){
    // Initiate wave tables.
    float *sqr_table = create_wavetable(TYPE_SQUARE, SAMPLE_RATE, 64);
    float *saw_table = create_wavetable(TYPE_SAW, SAMPLE_RATE, 64);
    float *sin_table = create_wavetable(TYPE_SINE, SAMPLE_RATE, 64);

    // Initiate oscilators.
    osc *osc1 = create_new_osc(sin_table, 0.5);
    osc *osc2 = create_new_osc(sin_table, 0.5);

    // Pack oscilators for transfer to callback function.
    osc_pack *oscillators = malloc(sizeof(osc_pack));
    oscillators->osc1 = osc1;
    oscillators->osc2 = osc2;

    // Start audio stream callback.
    PaStream *stream;
    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, pa_callback, oscillators);
    Pa_StartStream( stream );

    Pa_Sleep(20*1000);
    Pa_StopStream( stream );
    Pa_CloseStream( stream );

    free(osc1);
    free(osc2);
    free(oscillators);
    free(sqr_table);
    free(saw_table);
    free(sin_table);

    return 0;
}

/** Function called when audio buffer needs more data **/
static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ) {

    // Render first osc then add second one to it.
    osc_pack *oscillators = (osc_pack*)userData;
    render_osc(output, oscillators->osc1, frameCount, SAMPLE_RATE, 200.0);
    add_osc(output, oscillators->osc2, frameCount, SAMPLE_RATE, 300.0);

    return 0;
}
