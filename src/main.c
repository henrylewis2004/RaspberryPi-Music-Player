//standard C headers
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "my_debug.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"

//project headers
#include "led_manager.h"
#include "audio_player.h"
#include "sd_memory_manager.h"
#include "audio_dac_pin_values.h"
//#include "input.h"

// internal
void start_init(void){
	set_sys_clock_khz(150000,true); //150mhz / 10 = 15 MHz
	stdio_init_all();
	dma_claim_mask(0xfff); //claim all dma channels

	//led
	led_blink();
			       
	
	printf("audio_init\n");
	dma_unclaim_mask(0x30); //unclaim channels 4 & 5
	audio_init();

	
	printf("sd_functionality_test\n");
	dma_unclaim_mask(0x0f); //unclaim channels (0-3)
	sd_init();

	sleep_ms(10);
}

int64_t stop_playback_callback(alarm_id_t id, void *user_data){
	audio_stop_playback();
	return 0;
}

int64_t play_next_queue(alarm_id_t id, void *user_data){
	audio_skip_song();
	return 0;
}


// main \\

int main(void){
	start_init();
	
	audio_add_song_to_queue("californication/track01.cdda.wav");
	audio_add_song_to_queue("californication/track02.cdda.wav");
	audio_add_song_to_queue("californication/track03.cdda.wav");
	audio_add_song_to_queue("californication/track04.cdda.wav");
	audio_add_song_to_queue("californication/track05.cdda.wav");
	audio_add_song_to_queue("californication/track06.cdda.wav");
	audio_add_song_to_queue("californication/track07.cdda.wav");
	audio_add_song_to_queue("californication/track08.cdda.wav");
	audio_add_song_to_queue("californication/track09.cdda.wav");
	audio_add_song_to_queue("californication/track10.cdda.wav");
	audio_add_song_to_queue("californication/track11.cdda.wav");
	audio_add_song_to_queue("californication/track12.cdda.wav");
	audio_add_song_to_queue("californication/track13.cdda.wav");
	audio_add_song_to_queue("californication/track14.cdda.wav");
	audio_add_song_to_queue("californication/track15.cdda.wav");
	audio_play_top_queue();
	
	//add_alarm_in_ms(10000, &play_next_queue, NULL,true);
	//add_alarm_in_ms(20000, &stop_playback_callback, NULL,true);



	//infinite loop

	while(true){
		if (audio_buffer_refil_requested()){
			audio_buffer_refil();
		}

		__wfi();
	}

	
}
