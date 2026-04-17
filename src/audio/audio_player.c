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

#include "audio_song_queue.h"

#include "sd_memory_manager.h"
#include "string.h"



// internal \\

volatile bool buffer_refil_request;
volatile uint32_t* refil_buffer;

volatile float curvolume;

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

static void audio_play_song(char* filepath){
	i2s_set_buffer_callback(wav_buffer_callback);
	sd_set_playsong(filepath);
	
	for (int i = 0; i < DMA_CHANNEL_COUNT; i++){
		sd_wav_read_data(get_audio_buffer(i));
	}
	DAC_start_dma();
	dac_set_volume_quick(curvolume); //TODO: implement curvolume
	dac_mute(false);
	
}


//public


bool audio_buffer_refil_requested(void){
	return buffer_refil_request;
}

void audio_buffer_refil(void){
	buffer_refil_request = false;
	bool finished = sd_wav_read_data(refil_buffer);
	if (finished){
		audio_stop_playback();
		if (i2s_get_buffer_callback_function() == wav_buffer_callback){
			//sd_wav_close_playing_song();
			song_queue_goto_next_song();

			if (song_queue_top_has_song()){
				audio_play_top_queue();
			}
		}


	}
}


void audio_init(void){ 
	DAC_i2c_wakeup();
	DAC_i2s_init(wav_buffer_callback);

	curvolume = get_dac_volume();
}

void audio_close(void){
	// need to implement
	//DAC_powerdown();
	
	if (i2s_get_buffer_callback_function() == wav_buffer_callback){
		sd_wav_close_playing_song();
	}
	
	sd_close();
}

void audio_play_noise(void){
	i2s_set_buffer_callback(test_buffer_callback);
	DAC_start_dma();	
}


void audio_pause_song(void){
	DAC_toggle_pause();
}

void audio_stop_playback(void){
	dac_set_volume_quick(-100);
	dac_mute(true);
	DAC_pause_dma();
	sd_wav_close_playing_song();
}

void audio_volume_up(void){
	curvolume += AUDIO_VOLUME_UP;
	//ramp_set_dac_volume(curvolume, AUDIO_VOLUME_RAMP_STEPS, AUDIO_VOLUME_STEPS_TIMER_MS);
	dac_set_volume(curvolume);
}

void audio_volume_down(void){
	curvolume += AUDIO_VOLUME_DOWN;
	//ramp_set_dac_volume(curvolume, AUDIO_VOLUME_RAMP_STEPS, AUDIO_VOLUME_STEPS_TIMER_MS);
	dac_set_volume(curvolume);
}

void audio_skip_song(void){
	//NOTE: maybe change to ramp sound down instead?
	dac_set_volume_quick(-100);
	dac_mute(true);

	audio_stop_playback();
	song_queue_goto_next_song();

	audio_play_top_queue();
}

void audio_add_song_to_queue(char* filepath){
	song_queue_add_song(filepath);
}

int audio_play_top_queue(void){
	if (song_queue_get_top_song_path() != NULL){
		audio_play_song(song_queue_get_top_song_path());
		return 0;
	}
	panic("error playing top of song queue\n");
	return -1;
}
