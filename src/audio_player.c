//c libraries
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

//pico headers
#include "pico/stdlib.h"

//project libraries
#include "audio_player.h"
#include "audio_dac_bridge.h"

#ifndef SAMPLERATE
#define SAMPLE_RATE 44100
#endif

#ifndef SINE_FREQ 
#define SINE_FREQ 440
#endif

#ifndef BUF_SAMPLES 
#define BUF_SAMPLES 1024
#endif

// internal \\

int16_t sine_buf[BUF_SAMPLES];

static void generate_sine(void){
	for (int i = 0; i < BUF_SAMPLES; i++){
		sine_buf[i] = (int16_t)(32767.0f *
			sinf(2.0f * M_PI * SINE_FREQ * i / (float)SAMPLE_RATE)
		);

	}

}

//public

void audio_init(void){
	DAC_i2c_wakeup();
}

void audio_close(void){
	// need to implement
	DAC_powerdown();
}

void play_noise(void){
	// need to implement

}
