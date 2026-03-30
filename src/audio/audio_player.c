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

volatile bool buffer_refil_request;

// internal \\


static void wav_buffer_callback(uint32_t* buf){
	memcpy(buf,sd_get_next_samples(), I2S_BUFFER_WORDS * sizeof(uint32_t));

	buffer_refil_request = true;
}


static inline uint32_t i2s_pack_sample(int16_t left, int16_t right){
	return ((uint32_t)(uint16_t)left << 16) | (uint16_t)right;
}

static void test_buffer_callback(uint32_t *buf){
	static float phase = 0.0f;
	for (size_t i = 0; i < I2S_BUFFER_WORDS; i ++){
		int16_t s = (int16_t)(32767.0f * sin(phase));
		 buf[i] = i2s_pack_sample(s,s);
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
	sd_read_wav_data(sd_get_cur_buffer());
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
	sd_close();
}

void play_noise(void){
	i2s_set_buffer_callback(test_buffer_callback);
	DAC_start_dma();	
}

void play_song(char* filepath){
	i2s_set_buffer_callback(wav_buffer_callback);
	sd_set_playsong(filepath);
	DAC_start_dma();
}

void stop_playback(void){
	DAC_stop_dma();

	if (i2s_get_buffer_callback_function() == wav_buffer_callback){
		sd_wav_close_playing_song();
	}
}
