//pico headers
#include "pico/stdlib.h"

//project headers
#include "display_manager.h"
#include "display_hardware_bridge.h"
#include "display_oled_pin_values.h"
#include "screen_index.h"

// public \\

void display_init(void){
	display_hardware_init();
}

void display_test_run(void){
	display_hardware_set_powersave(false);
	display_hardware_clear_screen();

	/*
	//top bar
	display_hardware_draw_top_bar(true,0.8,"Main Menu",false);

	//main menu
	const char *items[] = SCREEN_MAIN_MENU_ITEMS;//{"Playing","Song Queue","Artists","Playlists","Options"};

	display_hardware_draw_list(SCREEN_MAIN_MENU,items,sizeof(items)/sizeof(items[0]),NULL,0,true);
	display_hardware_draw_list_selection_indicator(2, 0, SCREEN_MAIN_MENU);

	display_hardware_draw_screen();
	*/

	//top bar
	display_hardware_draw_top_bar(true,0.8,SCREEN_PLAYING_TEXT,false);

	//main menu
	display_hardware_draw_play_screen("My Chemical Romance", "The Black Parade","The Black Parade", 356, 0.5,false);

	display_hardware_draw_screen();

}

// goto pages
//NOTE: maybe change to pass single objects (song, album list, etc) and calulate instead of explicit seperate parameters
void display_goto_main_menu(void){
	//top bar
	display_hardware_draw_top_text(SCREEN_MAIN_MENU_TEXT, true);

	//main menu
	const char *items[] = SCREEN_MAIN_MENU_ITEMS;//{"Playing","Song Queue","Artists","Playlists","Options"};

	display_hardware_draw_list(SCREEN_MAIN_MENU,items,sizeof(items)/sizeof(items[0]),NULL,0,0,true);
	
	display_hardware_draw_screen();
}

void display_goto_playing(char *artist, char *album, char *title, uint seconds_remaining, float progress){
	//top bar
	display_hardware_draw_top_text(SCREEN_PLAYING_TEXT, true);

	//playing
	display_hardware_draw_play_screen(artist, album, title, seconds_remaining, progress, true);
	
	display_hardware_draw_screen();
}

void display_goto_song_queue(uint numbering_start,const char *queue[], size_t queue_length, const char *song_lengths[], size_t song_lengths_count){
	//top bar
	display_hardware_draw_top_text(SCREEN_QUEUE_TEXT, true);

	//queue list
	display_hardware_draw_list(SCREEN_QUEUE,queue,queue_length,song_lengths,song_lengths_count,numbering_start,true);
	
	display_hardware_draw_screen();
}

void display_goto_artist_select(const char *artists[], size_t artists_length){
	//top bar
	display_hardware_draw_top_text(SCREEN_ARTISTS_TEXT, true);

	//artist list
	display_hardware_draw_list(SCREEN_ARTISTS,artists,artists_length,NULL,0,0,true);
	
	display_hardware_draw_screen();
}

void display_goto_album_select(char *artist_name, const char *albums[],size_t albums_length,const char *years[], size_t years_count){
	//top bar
	display_hardware_draw_top_text(artist_name, true);

	//album list
	display_hardware_draw_list(SCREEN_ALBUMS,albums,albums_length,years,years_count,0,true);
	
	display_hardware_draw_screen();
}

void display_goto_title_select(uint numbering_start, char *album_name, const char *titles[],size_t titles_length,const char *song_lengths[], size_t song_lengths_count){
	//top bar
	display_hardware_draw_top_text(album_name, true);

	//title list
	display_hardware_draw_list(SCREEN_TITLES,titles,titles_length,song_lengths,song_lengths_count,numbering_start,true);
	
	display_hardware_draw_screen();
}

void display_goto_options(void){
	//top bar
	display_hardware_draw_top_text(SCREEN_OPTIONS_TEXT, true);

	//options list
	const char *options[] = SCREEN_OPTIONS_ITEMS;//{"Playing","Song Queue","Artists","Playlists","Options"};
						     //
	display_hardware_draw_list(SCREEN_OPTIONS,options,sizeof(options)/sizeof(options[0]),NULL,0,0,true);
	
	display_hardware_draw_screen();
}


//update display functions
void display_update_play_status(bool playing){
	display_hardware_draw_top_play_ind(playing, true);

	display_hardware_draw_screen();
}

void display_update_bat_level(float level){
	display_hardware_draw_top_bat_ind(level,true);

	display_hardware_draw_screen();
}

void display_update_play_progress(uint seconds_remaining,float progress){
	display_hardware_play_screen_update_time(seconds_remaining,progress, true);

	display_hardware_draw_screen();
}

void display_update_list_selection(uint selected_index, uint previous_index, uint SCREEN){
	display_hardware_clear_selection_ind(previous_index, SCREEN);
	display_hardware_draw_list_selection_indicator(selected_index, SCREEN_OPTIONS);

	display_hardware_draw_screen();
}

