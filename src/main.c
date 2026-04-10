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
#include "button_pins.h"
//#include "input.h"

static bool aliveMessage(struct repeating_timer *t){
	printf("I Breathe Father\n");
	return true;
}

// main \\

int64_t stop_playback_callback(alarm_id_t id, void *user_data){
	audio_stop_playback();
	return 0;
}

int64_t play_next_queue(alarm_id_t id, void *user_data){
	audio_skip_song();
	return 0;
}


int main(void){
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

	//setupButton(PLAY_PAUSE_PIN);

	//sleep_ms(1500);
	//printf("hello father\n");

	sleep_ms(100);
	
	//audio_play_song("track02.cdda.wav");
	
	audio_add_song_to_queue("track02.cdda.wav");
	audio_add_song_to_queue("track03.cdda.wav");
	audio_play_top_queue();
	
	//play_noise();


	
	//alive message
	struct repeating_timer alive_message_timer;
	//add_repeating_timer_ms(-5000,aliveMessage,NULL,&alive_message_timer);
	//add_repeating_timer_ms(-10000,pause_toggle,NULL,&alive_message_timer);
	add_alarm_in_ms(10000, &play_next_queue, NULL,true);
	add_alarm_in_ms(20000, &stop_playback_callback, NULL,true);



	//infinite loop

	while(true){
		if (audio_buffer_refil_requested()){
			audio_buffer_refil();
		}

		__wfi();
	}

	
}
