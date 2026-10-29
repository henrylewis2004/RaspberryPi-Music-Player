// c headers
#include <stdio.h>
#include <string.h>

// library headers
#include "pico/stdlib.h"
#include "u8g2.h"
#include "hardware/spi.h"

// project headers
#include "system_var.h"
#include "display_hardware_bridge.h"
#include "display_oled_pin_values.h"
#include "display_position_sizes.h"
#include "display_util.h"
#include "screen_index.h"

static u8g2_t u8g2;

//internal

//u8g2 callbacks for display setup

//u8x8 byte communication
uint8_t display_pico_byte_hw_spi_communication(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
	uint8_t *data;

	switch(msg){
		case U8X8_MSG_BYTE_SEND:
			data = (uint8_t *)arg_ptr;
			spi_write_blocking(DISPLAY_SPI_PORT,data, arg_int);
			break;

		case U8X8_MSG_BYTE_INIT:
			//might not be needed covered in other callback
			//u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
			break;

		case U8X8_MSG_BYTE_SET_DC:
			u8x8_gpio_SetDC(u8x8, arg_int);
			break;

		case U8X8_MSG_BYTE_START_TRANSFER:
			u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_enable_level);  
			break;

		case U8X8_MSG_BYTE_END_TRANSFER:
			u8x8_gpio_SetCS(u8x8, u8x8->display_info->chip_disable_level);
			break;

		default:
			return 0;
	}

	return 1;
}

//pico gpio and delay callback (implements communication delay and sets gpio values)
uint8_t display_pico_gpio_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr){
	switch(msg){
		//init
		case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8
			//spi	
			spi_init(DISPLAY_SPI_PORT, SYSTEM_BAUD_RATE);

			gpio_set_function(DISPLAY_CLK_GPIO_PIN,GPIO_FUNC_SPI); //clk
			gpio_set_function(DISPLAY_DIN_GPIO_PIN,GPIO_FUNC_SPI); //data
			
			//Chip Select 
			gpio_init(DISPLAY_CS_GPIO_PIN);
			gpio_set_dir(DISPLAY_CS_GPIO_PIN,GPIO_OUT);
			gpio_put(DISPLAY_CS_GPIO_PIN,true);
			
			//Data Command
			gpio_init(DISPLAY_DC_GPIO_PIN);
			gpio_set_dir(DISPLAY_DC_GPIO_PIN,GPIO_OUT);
			gpio_put(DISPLAY_DC_GPIO_PIN,true);

			//reset
			gpio_init(DISPLAY_RESET_GPIO_PIN);
			gpio_set_dir(DISPLAY_RESET_GPIO_PIN,GPIO_OUT);
			gpio_put(DISPLAY_RESET_GPIO_PIN,true);

			break;							
									
		//delay - NOTE: might have concurrency problems?
		case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
			busy_wait_us_32(10 * arg_int);
			break;
		case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
			sleep_ms(arg_int);
			break;

		//spi
		case U8X8_MSG_GPIO_SPI_CLOCK:
			gpio_put(DISPLAY_CLK_GPIO_PIN,arg_int);
			break;
		case U8X8_MSG_GPIO_SPI_DATA:
			gpio_put(DISPLAY_DIN_GPIO_PIN,arg_int);
			break;
		case U8X8_MSG_GPIO_CS:				// CS (chip select) pin: Output level in arg_int
			gpio_put(DISPLAY_CS_GPIO_PIN,arg_int);
			break;
		case U8X8_MSG_GPIO_DC:				// DC (data/cmd, A0, register select) pin: Output level in arg_int
			gpio_put(DISPLAY_DC_GPIO_PIN,arg_int);
			break;

		//reset
		case U8X8_MSG_GPIO_RESET:			// Reset pin: Output level in arg_int
			gpio_put(DISPLAY_RESET_GPIO_PIN,arg_int);
			break;

			/*
		case U8X8_MSG_GPIO_MENU_SELECT:
			u8x8_SetGPIOResult(u8x8, * get menu select pin state / 0);
			break;
		case U8X8_MSG_GPIO_MENU_NEXT:
			u8x8_SetGPIOResult(u8x8, * get menu next pin state / 0);
			break;
		case U8X8_MSG_GPIO_MENU_PREV:
			u8x8_SetGPIOResult(u8x8, * get menu prev pin state / 0);
			break;
		case U8X8_MSG_GPIO_MENU_HOME:
			u8x8_SetGPIOResult(u8x8, * get menu home pin state / 0);
			break;
			*/
		default:
			u8x8_SetGPIOResult(u8x8, 1);			// default return value
			break;
	}
	return 1;
}

//util

void hardware_clear_section(uint x, uint y, uint w, uint h){
	u8g2_SetDrawColor(&u8g2, 0);
	u8g2_DrawBox(&u8g2,x,y,w,h);

	u8g2_SetDrawColor(&u8g2, 1);
}

//top bar
//header text

void hardware_draw_top_text(char *top_text){
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	//artist
	char text[TOP_BAR_TEXT_MAX_LENGTH+1];
	text_size_sanitise(&u8g2,TOP_BAR_TEXT_MAX_LENGTH, top_text, text, sizeof(text));
	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,text))/2, TOP_BAR_TEXT_Y, text);

}

//play indicator
void hardware_draw_play(bool playing){ 
	if (playing){
		u8g2_DrawTriangle(&u8g2, PLAY_TRIANGLE_X0, PLAY_TRIANGLE_Y0, PLAY_TRIANGLE_X1, PLAY_TRIANGLE_Y1, PLAY_TRIANGLE_X2, PLAY_TRIANGLE_Y2);
	}
	else{
		u8g2_DrawBox(&u8g2,PAUSED_BOX_X, PAUSED_BOX_Y, PAUSED_BOX_W, PAUSED_BOX_H);
		u8g2_DrawBox(&u8g2,PAUSED_BOX_X + PAUSED_BOX_W + 2, PAUSED_BOX_Y, PAUSED_BOX_W, PAUSED_BOX_H);
	}
}

//battery
void hardware_draw_bat(u8g2_uint_t width){ //might need to change to float
	u8g2_DrawBox(&u8g2,BATTERY_IND_X, BATTERY_IND_Y, width, BATTERY_IND_H);
}

void hardware_draw_bat_level(float level){ //4 levels (need to calculate battery level before calling)
	u8g2_DrawFrame(&u8g2, BATTERY_IND_X, BATTERY_IND_Y, BATTERY_IND_W, BATTERY_IND_H);
	hardware_draw_bat((u8g2_uint_t)(BATTERY_IND_W * level));

}

// play screen
void hardware_draw_progress_bar(float progress){ 
	//borders
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y+PLAY_PROGRESS_V);
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_R_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_R_X, PLAY_PROGRESS_Y+PLAY_PROGRESS_V);

	//progress
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_LINE_X, PLAY_PROGRESS_LINE_Y, (u8g2_uint_t)(PLAY_PROGRESS_LINE_X_2 * progress), PLAY_PROGRESS_LINE_Y);

}

void hardware_draw_play_screen_song_text(char *artist, char *album, char *title){ 
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	//artist
	char text[PLAY_MAX_TEXT+1];
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, artist, text, sizeof(text));
	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,text))/2, PLAY_ARTIST_Y, text);

	//album
	text[0] = '\0';
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, album, text, sizeof(text));
	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,text))/2, PLAY_ALBUM_Y, text);

	//title
	text[0] = '\0';
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, title, text, sizeof(text));
	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,text))/2, PLAY_TITLE_Y, text);
}

void hardware_draw_play_screen_song_progress(char *time, float progress){ 
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	//artist
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,time))/2, PLAY_TIME_Y, time);

	hardware_draw_progress_bar(progress);
}

//lists

void hardware_draw_list(uint start_x, uint start_y, uint item_gap, const char *items[], size_t item_count, bool numbering, uint numbering_start,bool ending, bool centre){ //number = list each item with a number, ending = fill empty items with ---
	if (item_count <= 0 || items == NULL){
		return;
	}

	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	char text[LIST_MAX_TEXT_LENGTH+1];
	uint cnt = 0;
	while(cnt < LIST_MAX_COUNT && cnt < item_count && items[cnt] != NULL){
		text[0] = '\0';
		char intext[256];
		if (numbering){
			snprintf(intext,sizeof(intext),"%u. %s", cnt+numbering_start, items[cnt]);
		}else{
			snprintf(intext,sizeof(intext),"%s", items[cnt]);
		}

		text_size_sanitise(&u8g2,LIST_MAX_TEXT_LENGTH, intext, text, sizeof(text));

		if (centre){
			u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X - u8g2_GetStrWidth(&u8g2,text))/2, start_y + item_gap * cnt, text);
		}
		else{
			u8g2_DrawStr(&u8g2, start_x, start_y + item_gap * cnt, text);
		}


		cnt++;
	}

	if (ending && cnt < LIST_MAX_COUNT){
		for (int i = cnt; i < LIST_MAX_COUNT; i++){
			u8g2_DrawStr(&u8g2, start_x, start_y + item_gap * i, "---");
		}	
	}

}

void hardware_draw_list_secondary(uint x_edge_gap, uint start_y, uint item_gap, const char *items[],size_t item_count){  //album year + song length
	if (item_count <= 0 || items == NULL){
		return;
	}

	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	char text[LIST_MAX_SEC_TEXT_LENGTH+1];
	uint cnt = 0;
	while(cnt < LIST_MAX_COUNT && cnt < item_count && items[cnt] != NULL){
		text[0] = '\0';

		text_size_sanitise(&u8g2,LIST_MAX_TEXT_LENGTH, items[cnt], text, sizeof(text));
		u8g2_DrawStr(&u8g2, 128 - x_edge_gap - u8g2_GetStrWidth(&u8g2,text), start_y + item_gap * cnt, text);

		cnt++;
	}
	
}

void hardware_clear_selection_ind(uint start_x, uint start_y, uint item_gap, uint previous_index, bool mirror){
	hardware_clear_section(start_x, start_y + item_gap * previous_index,LIST_SELECTOR_W,LIST_SELECTOR_H);
	if (mirror){
		hardware_clear_section(128 - start_x - u8g2_GetStrWidth(&u8g2,DISPLAY_SELECTION_INDC_MIRROR), start_y + item_gap * previous_index,LIST_SELECTOR_W,LIST_SELECTOR_H);
	}
}

void hardware_selection_ind(uint start_x, uint start_y, uint item_gap, uint index, bool mirror){
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	u8g2_DrawStr(&u8g2, start_x, start_y + item_gap * index, DISPLAY_SELECTION_INDC);
	if (mirror){
		u8g2_DrawStr(&u8g2, 128 - start_x - u8g2_GetStrWidth(&u8g2,DISPLAY_SELECTION_INDC_MIRROR) , start_y + item_gap * index, DISPLAY_SELECTION_INDC_MIRROR);
	}
	
}


//public

void display_hardware_init(void){
	u8g2_Setup_sh1107_pimoroni_128x128_f(&u8g2,U8G2_R0,display_pico_byte_hw_spi_communication,display_pico_gpio_delay);

	u8g2_InitDisplay(&u8g2);
	//u8g2_SetPowerSave(&u8g2, 1); 
	display_hardware_set_powersave(true);
	u8g2_SetContrast(&u8g2, DISPLAY_CONTRAST); 

	u8g2_ClearBuffer(&u8g2);
	u8g2_SendBuffer(&u8g2);
}

void display_hardware_set_powersave(bool on){
	u8g2_SetPowerSave(&u8g2, (int)on); 
}


//top bar
void display_hardware_draw_top_bar(bool song_playing, float battery_level, char* top_text,bool clear){
	if (clear){
		hardware_clear_section(PLAY_TRIANGLE_X0,PLAY_TRIANGLE_Y0,128, TOP_BAR_Y);
	}

	hardware_draw_play(song_playing);
	hardware_draw_bat_level(battery_level);
	hardware_draw_top_text(top_text);
	u8g2_DrawHLine(&u8g2, TOP_BAR_X,TOP_BAR_Y,TOP_BAR_W);

}

void display_hardware_draw_top_play_ind(bool song_playing,bool clear ){
	if (clear){
		hardware_clear_section(PLAY_TRIANGLE_X0,PLAY_TRIANGLE_Y0,PLAY_TRIANGLE_X2 - PLAY_TRIANGLE_X0, PLAY_TRIANGLE_Y2);
	}

	hardware_draw_play(song_playing);
}

void display_hardware_draw_top_bat_ind(float battery_level,bool clear){
	if (clear){
		hardware_clear_section(BATTERY_IND_X,BATTERY_IND_Y,BATTERY_IND_W, BATTERY_IND_H);
	}

	hardware_draw_bat_level(battery_level);
}

void display_hardware_draw_top_text(char *text, bool clear){
	if (clear){
		hardware_clear_section(PLAY_TRIANGLE_X1 ,PLAY_TRIANGLE_Y0,BATTERY_IND_X - PLAY_TRIANGLE_X1, TOP_BAR_Y);
	}

	hardware_draw_top_text(text);
}

//play screen
void display_hardware_draw_play_screen(char *artist, char *album, char *title, uint seconds_remaining, float progress, bool clear){
	if (clear){
		//text
		hardware_clear_section(0,TOP_BAR_Y+1,128,PLAY_TITLE_Y); 
		//progress
		hardware_clear_section(PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_R_X - PLAY_PROGRESS_L_X, PLAY_PROGRESS_V);
	}

	char time_remaining[PLAY_PROGRESS_TEXT_MAX];
	time_sanitise(seconds_remaining,time_remaining,sizeof(time_remaining));

	hardware_draw_play_screen_song_text(artist,album,title);
	hardware_draw_play_screen_song_progress(time_remaining, progress);
}

void display_hardware_play_screen_update_time( uint seconds_remaining,float progress, bool clear){
	if (clear){
		hardware_clear_section(PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_R_X - PLAY_PROGRESS_L_X, 128 - PLAY_PROGRESS_Y); //NOTE: might want to just clear buffer instead
	}

	char time_remaining[PLAY_PROGRESS_TEXT_MAX];
	time_sanitise(seconds_remaining,time_remaining,sizeof(time_remaining));

	hardware_draw_play_screen_song_progress(time_remaining, progress);
}

//list
void display_hardware_draw_list(uint SCREEN, const char *items[], size_t item_count, const char *secondary_items[], size_t secondary_items_count, uint numbering_start, bool clear){
	if (clear){
		display_hardware_clear_list_area(LIST_ITEM_X0);
	}
	if (SCREEN == SCREEN_PLAYING){
		return;
	}
	else if (SCREEN==SCREEN_MAIN_MENU){
		hardware_draw_list(LIST_ITEM_X0,LIST_MAIN_MENU_ITEM_Y0,LIST_MAIN_MENU_ITEM_GAP, items,item_count,false,0,false,true);
	}
	else{
		bool numbering = false;
		bool ending = true;
		bool centre = false;

		if (SCREEN == SCREEN_QUEUE || SCREEN == SCREEN_TITLES ){
			numbering = true;
		}
		else if (SCREEN == SCREEN_OPTIONS){
			ending = false;
		}

		hardware_draw_list(LIST_ITEM_X0,LIST_ITEM_Y0,LIST_ITEM_GAP, items,item_count,numbering,numbering_start,ending,centre);

		if (secondary_items != NULL){
			hardware_draw_list_secondary(LIST_SEC_ITEM_EDGE_GAP, LIST_ITEM_Y0, LIST_ITEM_GAP, secondary_items,secondary_items_count);  //album year + song length
		}
	}
}

void display_hardware_clear_list_area(uint x){
	hardware_clear_section(x,TOP_BAR_Y+1,128,128); 
}


// selection indicator
void display_hardware_draw_list_selection_indicator(uint index, uint SCREEN){
	if (SCREEN == SCREEN_MAIN_MENU){ 
		hardware_selection_ind(LIST_SELECTION_INDC_X, LIST_MAIN_MENU_ITEM_Y0, LIST_MAIN_MENU_ITEM_GAP,index,true);
	}
	else{
		hardware_selection_ind(LIST_SELECTION_INDC_X, LIST_ITEM_Y0, LIST_ITEM_GAP,index,false);
	}
}

void display_hardware_clear_selection_ind(uint index, uint SCREEN){
	if (SCREEN==SCREEN_MAIN_MENU){
		hardware_clear_selection_ind(LIST_SELECTION_INDC_X,LIST_MAIN_MENU_ITEM_Y0,LIST_MAIN_MENU_ITEM_GAP,index,true);
	}
	else{
		hardware_clear_selection_ind(LIST_SELECTION_INDC_X,LIST_MAIN_MENU_ITEM_Y0,LIST_MAIN_MENU_ITEM_GAP,index,false);
	}
}

// clear + send functions
void display_hardware_clear_screen(void){
	u8g2_ClearBuffer(&u8g2);
}

void display_hardware_draw_screen(void){
	u8g2_SendBuffer(&u8g2);
}
