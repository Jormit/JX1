#include <stdio.h>
#include <math.h>
#include "include/portaudio.h"

#define SAMPLE_RATE (44100)

typedef struct {
    float left_phase;
    float right_phase;
} saw_osc;

/****Function called when audio buffer needs more data****/
static int PaCallback( const void *input, void *output, unsigned long frameCount,
                     const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ) {
    saw_osc *data = (saw_osc*)userData;
    float *out = (float*)output;

    unsigned int i;
    for (i=0; i < frameCount; i++){
        *out++ = data->left_phase;
        *out++ = data->left_phase;

        data->left_phase += 0.01f;
        data->right_phase += 0.01f;

        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;
}

int main(void){
    saw_osc saw;
    PaStream *stream;

    saw.left_phase = saw.right_phase = 0.0;

    Pa_Initialize();
    Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256, PaCallback, &saw);
    Pa_StartStream( stream );

    Pa_Sleep(2*1000);
    Pa_StopStream( stream );
    Pa_CloseStream( stream );
}
