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
#include "controller.h"
#include "system_var.h"
#include "led_manager.h"
#include "audio_player.h"
#include "sd_memory_manager.h"
#include "audio_dac_pin_values.h"
#include "display_manager.h"
#include "input.h"
#include "screen_index.h"
#include "library_manager.h"

typedef struct {
	//input
	input_flags input_button_flags;
	uint32_t last_input_ts; //note 32 wraps every 1 hour 11 minutes 35 seconds, should be fine for input cooldown? alternatively cna use 64

	//pages
	uint8_t cur_page;

	//selected indexes
	uint32_t selected_index;

	//info
	char* cur_artist;
	char* cur_album;
	char* cur_song;
	

} state;

static state cur_state;

// private

void reset_state(state s){
	s.input_button_flags = input_reset_flags();
	s.last_input_ts = 0;

	s.cur_page = SCREEN_DEFAULT_INIT;
	s.selected_index = 0;

	s.cur_artist = NULL;
	s.cur_album = NULL;
	s.cur_song = NULL;
}

void reset_state_song_info(state s){
	s.cur_artist = NULL;
	s.cur_album = NULL;
	s.cur_song = NULL;
}



//pages
void goto_page(uint8_t page){
	switch(page){
		//menus
		case SCREEN_MAIN_MENU:
			display_goto_main_menu();
			break;
		case SCREEN_OPTIONS:
			break;
		case SCREEN_PLAYING:
			break;
		case SCREEN_QUEUE:
			break;

		//songs
		case SCREEN_ARTISTS:
			break;
		case SCREEN_ALBUMS:
			break;
		case SCREEN_TITLES:
			break;

		//default
		default:
			printf("error: goto_page tried incorrect page index (page=%u)\n", page);
			return;
	}

	cur_state.cur_page = page;
	cur_state.selected_index = 0;
}

void goto_previous_page(void){
	if (cur_state.cur_page == SCREEN_MAIN_MENU){
		return;
	}

	if (cur_state.cur_page == SCREEN_PLAYING || cur_state.cur_page == SCREEN_ARTISTS || cur_state.cur_page == SCREEN_OPTIONS){
		cur_state.cur_page = SCREEN_MAIN_MENU;
	}
	else if (cur_state.cur_page == SCREEN_ALBUMS || cur_state.cur_page == SCREEN_TITLES || cur_state.cur_page == SCREEN_QUEUE){
		cur_state.cur_page--;
	}

	goto_page(cur_state.cur_page);
}

//index
uint32_t get_max_index(void){
	switch(cur_state.cur_page){
		//constant
		case SCREEN_MAIN_MENU:
			return SCREEN_MAIN_MENU_ITEMS_COUNT;
		case SCREEN_OPTIONS:
			return SCREEN_OPTIONS_ITEMS_COUNT;

		//dynamic
		case SCREEN_ARTISTS:
			return library_get_artist_count();
		case SCREEN_ALBUMS:
			return library_get_artist_album_count(cur_state.cur_artist);
		case SCREEN_TITLES:
			return library_get_album_song_count(cur_state.cur_artist, cur_state.cur_album);
		case SCREEN_QUEUE:
			return audio_get_song_queue_count();

		default:
			return 0;
			 
	}
}

void index_select_up(void){
	if (cur_state.selected_index == 0){
		return;
	}

	display_update_list_selection(cur_state.selected_index - 1,cur_state.selected_index,cur_state.cur_page);
	cur_state.selected_index--;
}

void index_select_down(void){
	if (cur_state.selected_index + 1 >= get_max_index()){ 
		return;
	}

	display_update_list_selection(cur_state.selected_index + 1,cur_state.selected_index,cur_state.cur_page);
	cur_state.selected_index++;
}

// public

void controller_system_init(void){
	set_sys_clock_khz(SYSTEM_CLOCK_RATE,true); //150mhz / 10 = 15 MHz
	stdio_init_all();
	dma_claim_mask(0xfff); //claim all dma channels

	//led
	led_blink();
	sleep_ms(5000);
	printf("first i breathe!\n");
			       
	/*
	//audio
	printf("audio_init\n");
	dma_unclaim_mask(0x30); //unclaim channels 4 & 5
	audio_init();
	*/

	//sd card
	printf("sd_init\n");
	dma_unclaim_mask(0x0f); //unclaim channels (0-3)
	sd_init();
	/*

	//input
	printf("input init\n");
	input_init();

	*/
	//display 
	printf("display init\n");
	display_init();

	sleep_ms(10);

	reset_state(cur_state);
	//note: maybe change to loading or init screen?
	goto_page(SCREEN_MAIN_MENU);
}

void controller_system_loop(void){
	//audio_song_test(true);

	//infinite loop
	while(true){
		cur_state.input_button_flags = input_get_flags();

		if (audio_buffer_refil_requested()){
			audio_buffer_refil();
		}

		//input 
		//NOTE: maybe change timestamp method to an alarm (not sure how input callback is called, once per action or whilst held down?)
		
		if (time_us_32() - cur_state.last_input_ts >= INPUT_COOLDOWN_US){
			if (cur_state.input_button_flags.play_button){
				//input_button_flags.play_button = false;
				printf("Play_pause_song called\n");

				audio_play_pause_song();
			}
			if (cur_state.input_button_flags.axis_right){
				//input_button_flags.axis_right = false;
				printf("axis right button called\n");
			}
			if (cur_state.input_button_flags.axis_left){
				//input_button_flags.axis_back = false;
				printf("axis back button called\n");

				goto_previous_page();
			}
			if (cur_state.input_button_flags.axis_up){
				//input_button_flags.axis_up = false;
				printf("axis up button called\n");

				index_select_up();
			}
			if (cur_state.input_button_flags.axis_down){
				//input_button_flags.axis_down = false;
				printf("axis down button called\n");

				index_select_down();
			}

			cur_state.input_button_flags = input_reset_flags();
			cur_state.last_input_ts = time_us_32();
		}

		__wfi();
	
	}

}


