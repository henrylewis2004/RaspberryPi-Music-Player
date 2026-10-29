#ifndef DISPLAY_HARDWARE_BRIDGE_H
#define DISPLAY_HARDWARE_BRIDGE_H

void display_hardware_init(void);
void display_hardware_draw_test(void);

void display_hardware_draw_top_bat_ind(float battery_level,bool clear);
void display_hardware_draw_top_play_ind(bool song_playing,bool clear);

void display_hardware_draw_top_bar(bool song_playing, float battery_level,bool clear);
void display_hardware_draw_play_screen(char *artist, char *album, char *title, char *time_text, float progress, bool clear);
void display_hardware_play_screen_update_time(char *time_text, float progress, bool clear);

void display_hardware_draw_screen(void);
void display_hardware_clear_screen(void);

#endif
