//c libraries
#include <stdbool.h>
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"

//project libraries
#include "audio_player.h"
#include "audio_dac_i2c_bridge.h"
#include "audio_dac_i2s_bridge.h"

#ifndef DAC_SAMPLE_RATE
#define DAC_SAMPLE_RATE 44100
#endif


// internal \\



//public

void audio_init(void){
	DAC_i2c_wakeup();
	printf("wakeup finished\n");
	DAC_i2s_init();
	printf("i2s init finished\n");
}

void audio_close(void){
	// need to implement
	DAC_powerdown();
}

void play_noise(void){
	DAC_start_dma();	
}

void stop_playback(void){
}
