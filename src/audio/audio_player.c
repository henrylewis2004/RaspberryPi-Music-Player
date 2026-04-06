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
#include "audio_dac_i2s_values.h"

#include "sd_memory_manager.h"
#include "string.h"


// internal \\

volatile bool buffer_refil_request;
volatile uint32_t* refil_buffer;

static void wav_buffer_callback(uint32_t* buf){
	buffer_refil_request = true;
	refil_buffer = buf;
}


static void test_buffer_callback(uint32_t *buf){
	static float phase = 0.0f;
	for (size_t i = 0; i < I2S_BUFFER_WORDS; i ++){
		int16_t s = (int16_t)(32767.0f * sin(phase));
		 buf[i] =((uint32_t)(uint16_t)s << 16) | (uint16_t)s; 
		 phase += 2.0f * M_PI * 440.0f / DAC_SAMPLE_RATE;
		 if(phase > 2.0f * M_PI){
			phase -= 2.0f * M_PI;

		 }
	}
}

//public




bool audio_buffer_refil_requested(void){
	return buffer_refil_request;
}

void audio_buffer_refil(void){
	buffer_refil_request = false;
	bool finished = sd_wav_read_data(refil_buffer);
	if (finished){
		if (i2s_get_buffer_callback_function() == wav_buffer_callback){
			sd_wav_close_playing_song();
		}
		stop_playback();
	}
}





void audio_init(void){
	DAC_i2c_wakeup();
	printf("wakeup finished\n");
	DAC_i2s_init(wav_buffer_callback);
	printf("i2s init finished\n");
}

void audio_close(void){
	// need to implement
	//DAC_powerdown();
	
	if (i2s_get_buffer_callback_function() == wav_buffer_callback){
		sd_wav_close_playing_song();
	}
	
	sd_close();
}

void play_noise(void){
	i2s_set_buffer_callback(test_buffer_callback);
	DAC_start_dma();	
}

void play_song(char* filepath){
	i2s_set_buffer_callback(wav_buffer_callback);
	sd_set_playsong(filepath);
	
	for (int i = 0; i < DMA_CHANNEL_COUNT; i++){
		sd_wav_read_data(get_audio_buffer(i));
	}
	DAC_start_dma();
	
}

void stop_playback(void){
	DAC_stop_dma();
	sd_close();
}

void audio_plause(bool pause){
	if pause{
		DAC_stop_dma();
	}
	else{
		DAC_start_dma();
	}
}

