// library headers
#include "pico/stdlib.h"
#include "u8g2.h"
#include "hardware/spi.h"

// project headers
#include "system_var.h"
#include "display_hardware_bridge.h"
#include "display_oled_pin_values.h"

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

//public

void display_hardware_init(void){
	u8g2_Setup_sh1107_pimoroni_128x128_f(&u8g2,U8G2_R1,display_pico_byte_hw_spi_communication,display_pico_gpio_delay);

	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 1); 

	u8g2_ClearBuffer(&u8g2);
}

void display_hardware_draw_test(void){
	u8g2_ClearBuffer(&u8g2);

	// Draw some text and graphics.
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_DrawStr(&u8g2, 0, 10, "Hello Pico!");

	u8g2_DrawFrame(&u8g2, 0, 30, 118, 60);
	u8g2_DrawCircle(&u8g2, 64, 90, 15, U8G2_DRAW_ALL);

	// Transfer the framebuffer to the display.
	u8g2_SendBuffer(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0); 
}


