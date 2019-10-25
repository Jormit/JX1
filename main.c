#include <stdio.h>
#include <math.h>
#include "include/portaudio.h"

// Synth defines.
#include "wavetables.h"
#include "osc.h"

#define SAMPLE_RATE (44100)

static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );

int main(void){
    /* float *sin_table = create_wavetable(TYPE_SINE, SAMPLE_RATE, 0);
    osc *sin_osc = create_new_osc(sin_table); */

    /* float *square_table = create_wavetable(TYPE_SQUARE, SAMPLE_RATE, 0);
    osc *sqr_osc = create_new_osc(square_table); */

    float *saw_table = create_wavetable(TYPE_SAW, SAMPLE_RATE, 64);
    osc *saw_osc = create_new_osc(saw_table);

    PaStream *stream;

    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, pa_callback, saw_osc);
    Pa_StartStream( stream );

    Pa_Sleep(20*1000);
    Pa_StopStream( stream );
    Pa_CloseStream( stream );

    return 0;
}

/** Function called when audio buffer needs more data **/
static int pa_callback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ) {

    render_osc(output, userData, frameCount, SAMPLE_RATE, 261.6);

    return 0;
}
