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


//top bar
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
void hardware_draw_play_progress(float progress){ 
	//borders
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_L_X, PLAY_PROGRESS_Y+PLAY_PROGRESS_V);
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_R_X, PLAY_PROGRESS_Y, PLAY_PROGRESS_R_X, PLAY_PROGRESS_Y+PLAY_PROGRESS_V);

	//progress
	u8g2_DrawLine(&u8g2, PLAY_PROGRESS_LINE_X, PLAY_PROGRESS_LINE_Y, (u8g2_uint_t)(PLAY_PROGRESS_LINE_X_2 * progress), PLAY_PROGRESS_LINE_Y);

}

void hardware_draw_play_screen_song_text(char *artist, char *album, char *title, char *time){ 
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);

	//artist
	char text[PLAY_MAX_TEXT];
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, artist, text, sizeof(text));
	
	u8g2_uint_t str_length = u8g2_GetStrWidth(&u8g2,text)/2;	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X/2) - str_length, PLAY_ARTIST_Y, text);

	//album
	text[0] = '\0';
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, album, text, sizeof(text));
	
	str_length = u8g2_GetStrWidth(&u8g2,text)/2;	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X/2) - str_length, PLAY_ALBUM_Y, text);

	//title
	text[0] = '\0';
	text_size_sanitise(&u8g2,PLAY_MAX_TEXT, title, text, sizeof(text));
	
	str_length = u8g2_GetStrWidth(&u8g2,text)/2;	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X/2) - str_length, PLAY_TITLE_Y, text);

	//time
	str_length = u8g2_GetStrWidth(&u8g2,time)/2;	
	u8g2_DrawStr(&u8g2, (DISPLAY_SIZE_X/2) - str_length, PLAY_TIME_Y, time);

}

//public

void display_hardware_init(void){
	u8g2_Setup_sh1107_pimoroni_128x128_f(&u8g2,U8G2_R0,display_pico_byte_hw_spi_communication,display_pico_gpio_delay);

	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 1); 
	u8g2_SetContrast(&u8g2, 255); 

	u8g2_ClearBuffer(&u8g2);
	u8g2_SendBuffer(&u8g2);
}

void display_hardware_draw_test(void){
	u8g2_ClearBuffer(&u8g2);

	hardware_draw_play(true);
	hardware_draw_bat_level(0.5);
	hardware_draw_play_progress(0.7);
	hardware_draw_play_screen_song_text("Red Hot Chili Peppers", "Californication", "Around the World","-01:24");

	// Transfer the framebuffer to the display.
	u8g2_SendBuffer(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0); 
}

void display_hardware_top_bar(bool song_playing, float battery_level){
	//add clear top row buffer
	hardware_draw_play(song_playing);
	hardware_draw_bat_level(battery_level);

	u8g2_SendBuffer(&u8g2);
}

void display_hardware_play_screen(char *artist, char *album, char *title, char *time_text, float progress){
	//add clear row buffer
	hardware_draw_play_screen_song_text(artist,album,title,time_text);
	hardware_draw_play_progress(progress);

	u8g2_SendBuffer(&u8g2);
}
