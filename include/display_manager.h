#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

void display_init(void);
void display_test_run(void);

// goto pages
void display_goto_main_menu(uint selected_index);
void display_goto_playing(char *artist, char *album, char *title, uint seconds_remaining, float progress);//NOTE: maybe change to pass a song object instead of seperate parameters
void display_goto_song_queue(uint selected_index, uint numbering_start,const char *queue[], size_t queue_length, const char *song_lengths[], size_t song_lengths_count);
void display_goto_artist_select(uint selected_index, const char *artists[], size_t artists_length);
void display_goto_album_select(uint selected_index, char *artist_name, const char *albums[],size_t albums_length,const char *years[], size_t years_count);
void display_goto_title_select(uint selected_index, uint numbering_start, char *album_name, const char *titles[],size_t titles_length,const char *song_lengths[], size_t song_lengths_count);
void display_goto_options(uint selected_index);

//update display functions
void display_update_play_status(bool playing);
void display_update_bat_level(float level);
void display_update_play_progress(uint seconds_remaining,float progress);


#endif
