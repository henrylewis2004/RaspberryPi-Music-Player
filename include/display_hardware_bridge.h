#ifndef DISPLAY_HARDWARE_BRIDGE_Hdisp
#define DISPLAY_HARDWARE_BRIDGE_H

#ifndef DISPLAY_SELECTION_INDC
#define DISPLAY_SELECTION_INDC ">"
#endif

#ifndef DISPLAY_SELECTION_INDC_MIRROR
#define DISPLAY_SELECTION_INDC_MIRROR "<"
#endif

#ifndef DISPLAY_CONTRAST
#define DISPLAY_CONTRAST 64
#endif


//general
void display_hardware_init(void);

void display_hardware_draw_screen(void);
void display_hardware_clear_screen(void);

void display_hardware_set_powersave(bool on);

//top bar
void display_hardware_draw_top_bar(bool song_playing, float battery_level, char *top_text,bool clear);
void display_hardware_draw_top_bat_ind(float battery_level,bool clear);
void display_hardware_draw_top_play_ind(bool song_playing,bool clear);
void display_hardware_draw_top_text(char *text, bool clear);

//play screen
void display_hardware_draw_play_screen(char *artist, char *album, char *title, uint seconds_remaining, float progress, bool clear);
void display_hardware_play_screen_update_time(uint seconds_remaining,float progress,bool clear);

//lists
void display_hardware_draw_list(uint SCREEN, const char *items[], size_t item_count, const char *secondary_items[], size_t secondary_items_count, uint numbering_start,bool clear);
void display_hardware_clear_list_area(uint x);

//selection indicator
void display_hardware_draw_list_selection_indicator(uint index, uint SCREEN);
void display_hardware_clear_selection_ind(uint index, uint SCREEN);


#endif
