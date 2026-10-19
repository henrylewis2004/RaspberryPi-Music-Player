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
#include "display_manager.h"
#include "input.h"

static input_flags input_button_flags;

// internal
void start_init(void){
	set_sys_clock_khz(150000,true); //150mhz / 10 = 15 MHz
	stdio_init_all();
	dma_claim_mask(0xfff); //claim all dma channels

	//led
	led_blink();
	sleep_ms(5000);
	printf("first i breathe!\n");
			       
	//audio
	printf("audio_init\n");
	dma_unclaim_mask(0x30); //unclaim channels 4 & 5
	audio_init();

	//sd card
	printf("sd_init\n");
	dma_unclaim_mask(0x0f); //unclaim channels (0-3)
	sd_init();

	//display_init();
	
	//input
	printf("input init\n");
	input_init();

	sleep_ms(10);
}

// main \\

int main(void){
	start_init();

	printf("add songs to queue\n");
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

	audio_add_song_to_queue("Revolver/track01.wav");
	audio_add_song_to_queue("Revolver/track03.wav");
	audio_add_song_to_queue("Revolver/track02.wav");
	audio_add_song_to_queue("Revolver/track04.wav");
	audio_add_song_to_queue("Revolver/track05.wav");
	audio_add_song_to_queue("Revolver/track06.wav");
	audio_add_song_to_queue("Revolver/track07.wav");
	audio_add_song_to_queue("Revolver/track08.wav");
	audio_add_song_to_queue("Revolver/track09.wav");
	audio_add_song_to_queue("Revolver/track10.wav");
	audio_add_song_to_queue("Revolver/track11.wav");
	audio_add_song_to_queue("Revolver/track12.wav");
	audio_add_song_to_queue("Revolver/track13.wav");
	audio_add_song_to_queue("Revolver/track14.wav");
	audio_add_song_to_queue("Revolver/track15.wav");

	audio_play_top_queue();
	
	//display_test_run();

	//infinite loop

	while(true){
		input_button_flags = input_get_flags();

		if (audio_buffer_refil_requested()){
			audio_buffer_refil();
		}
		if (input_button_flags.play_button){
			//input_button_flags.play_button = false;
			audio_play_pause_song();

		}

		input_button_flags = input_get_reset_flags();

		__wfi();
	
	}

	
}
