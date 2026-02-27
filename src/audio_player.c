//c libraries
#include <stdbool.h>
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#if USE_AUDIO_I2S
#include "pico/audio_i2s.h"
#endif

//project libraries
#include "audio_player.h"
#include "audio_dac_values.h"

// internal \\

//register writing methods
static int dac_reg_write(uint8_t reg, uint8_t val){
	uint8_t buffer[2] = {reg,val};
	return i2c_write_blocking(I2C_PORT,DAC_ADDR, buffer, 2, false);
}

static int dac_reg_page(uint8_t page){
	return dac_reg_write(0x00,page);
}

//dac methods
static void dac_i2c_start(void){
#if !defined(I2C_PORT) || !defined(DAC_SDA_GPIO_PIN) || !defined(DAC_SCL_GPIO_PIN)
	#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined\n");
#else
	i2c_init(I2C_PORT,100*1000);

	gpio_set_function(DAC_SDA_GPIO_PIN,GPIO_FUNC_I2C);
	gpio_set_function(DAC_SCL_GPIO_PIN,GPIO_FUNC_I2C);

	gpio_pull_up(DAC_SDA_GPIO_PIN);
	gpio_pull_up(DAC_SCL_GPIO_PIN);
	
#endif
}

static bool dac_addr_response(void){
	uint8_t dummy;
	int result = i2c_read_blocking(I2C_PORT,DAC_ADDR,&dummy,1,false);
	return (result >= 0);
}

static void dac_reset(void){
	//reset hardware - pullup reset pin
	
	//setup reset pin
	gpio_init(DAC_RESET_GPIO_PIN);
	gpio_set_dir(DAC_RESET_GPIO_PIN,GPIO_OUT);

	//reset dac
	gpio_put(DAC_RESET_GPIO_PIN,false);
	sleep_ms(100);
	gpio_put(DAC_RESET_GPIO_PIN,true);
	sleep_ms(100);
}

static int dac_register_setup(void){
	dac_init();
	dac_configure_clocks();

	//route DAC to output
	if (dac_reg_write(0x3F,0xD4) == PICO_ERROR_GENERIC){
		panic("reg - 0x3F error:(%d)\n", res);
		return -1;
	}

	//unmute
	dac_mute(false);

	printf("finished reg setup\n");
}

static int dac_init(void){
	//set page 0
	if (dac_reg_page(DAC_REG_PG0) == PICO_ERROR_GENERIC){
		panic("reg - 0x00 error:(%d)\n", res);
		return -1;
	}

	//reset registers
	if (dac_reg_write(0x01,0x01) == PICO_ERROR_GENERIC){
		panic("reg - 0x01 error:(%d)\n", res);
		return -1;
	}
	sleep_ms(10);

	//set volumes low
	if (set_channel_volume(true,-63.5) == PICO_ERROR_GENERIC){ //right
		panic("reg - 0x01 error:(%d)\n", res);
		return -1;
	}
	if (set_channel_volume(false,-63.5) == PICO_ERROR_GENERIC){ //right
		panic("reg - 0x01 error:(%d)\n", res);
		return -1;
	}

	//configure dac output
	//L + R enabled & I2S enabled
	dac_reg_write(DAC_DATA_PATH_REG,DAC_DATA_PATH_VALUE);
}


static int set_channel_volume(bool right_channel, float volume_db){
	//right channel = 0x42, left channel = 0x41
	const uint8_t channel_register = (right_channel)? DAC_RIGHT_CHANNEL_REG : DAC_LEFT_CHANNEL_REG;

	if (volume_db > 24.0){
		volume_db = 24.0;
	}
	else if (volume_db < -63.5){
		volume_db = -63.5;
	}

	int reg_value = (int)(volume_db * 2);
	if (reg_value == 0x80 || reg_value > 0x30){
		panic("INCORRECT set_channel_volume register volume - volume: %d", reg_value);
		return -1;
	}

	return dac_reg_write(channel_register,reg_value & 0xFF);
}

static int dac_mute(bool mute){
	if (mute){ //mute L + R
		if (dac_reg_write(DAC_VOLUME_CONTROL,0x0C) == PICO_ERROR_GENERIC){
			panic("reg - 0x40 error:(%d)\n", res);
			return -1;
		}
	}
	else{ //unmute
		if (dac_reg_write(DAC_VOLUME_CONTROL,0x00) == PICO_ERROR_GENERIC){
			panic("reg - 0x40 error:(%d)\n", res);
			return -1;
		}
	}
	return 0;
}

static int dac_configure_clocks(void){
	const uint sample_rate = DAC_SAMPLE_RATE;
	const uint bit_depth = DAC_BIT_DEPTH;

	return 0;
}

//wakeup handshake
static void DAC_i2c_wakeup(){
	printf("dac wakeup\n");
	dac_i2c_start();
	dac_reset();

	
	if (dac_addr_response()){
		printf("DAC responsed to 0x%02X\n", DAC_ADDR);
		dac_register_setup();
	}
	else{
		printf("no DAC response to 0x%02X\n", DAC_ADDR);
	}
}

static void DAC_powerdown(){
}


//public
void i2c_bus_scan(void){
	printf("scan starting\n");
	uint8_t dummy;
	for (uint8_t addr = 0; addr< 128; addr++){
		int result = i2c_read_blocking(I2C_PORT,addr,&dummy,1,false);
		if (result >= 0){
			printf("Found Device at 0x%02X\n",addr);
		}
	}
	printf("scan finished\n");
}

void audio_init(void){
	DAC_i2c_wakeup();
}

void play_noise(void){

}
