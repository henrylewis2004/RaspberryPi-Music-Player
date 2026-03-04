//c libraries
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

//pico headers
#include "pico/stdlib.h"

//project libraries
#include "audio_player.h"
#include "audio_dac_i2c_bridge.h"
#include "audio_dac_i2s_bridge.h"

#ifndef DAC_SAMPLE_RATE
#define DAC_SAMPLE_RATE 44100
#endif

#ifndef SINE_FREQ 
#define SINE_FREQ 440
#endif

#ifndef BUF_SAMPLES 
#define BUF_SAMPLES 1024
#endif

// internal \\


//public

void audio_callback(uint32_t *buf, size_t count){
	static float phase = 0.0f;
	for (size_t i = 0; i < count; i ++){
		int16_t s = (int16_t)(32767.0f * sin(phase));
		buf[i] = i2s_pack_sample(s,s);
		phase += 2.0f * M_PI * 440.0f / DAC_SAMPLE_RATE;
		if (phase > 2.0f * M_PI){
			phase -= 2.0f * M_PI;
		}
	}
}

void audio_init(void){
	DAC_i2c_wakeup();
	DAC_i2s_init(audio_callback);
}

void audio_close(void){
	// need to implement
	DAC_powerdown();
}

void play_noise(void){
	// need to implement

}
