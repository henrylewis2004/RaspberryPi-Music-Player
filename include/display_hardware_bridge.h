#ifndef DISPLAY_HARDWARE_BRIDGE_Hdisp
#define DISPLAY_HARDWARE_BRIDGE_H

#ifndef DISPLAY_SELECTION_INDC
#define DISPLAY_SELECTION_INDC ">"
#endif

#ifndef DISPLAY_SELECTION_INDC_MIRROR
#define DISPLAY_SELECTION_INDC_MIRROR "<"
#endif

//general
void display_hardware_init(void);
void display_hardware_draw_test(void);

void display_hardware_draw_screen(void);
void display_hardware_clear_screen(void);

//top bar
void display_hardware_draw_top_bar(bool song_playing, float battery_level,bool clear);
void display_hardware_draw_top_bat_ind(float battery_level,bool clear);
void display_hardware_draw_top_play_ind(bool song_playing,bool clear);
void display_hardware_draw_top_text(char *text);

//play screen
void display_hardware_draw_play_screen(char *artist, char *album, char *title, char *time_text, float progress, bool clear);
void display_hardware_play_screen_update_time(char *time_text, float progress, bool clear);

//lists
void display_hardware_draw_list(uint SCREEN, const char *items[], size_t item_count, const char *secondary_items[], size_t secondary_items_count, bool clear);
void display_hardware_clear_list_area(uint x);

//selection indicator
void display_hardware_draw_list_selection_indicator(uint index, uint previous_index, uint SCREEN);
void display_hardware_clear_selection_ind(uint index, uint SCREEN);


#endif
