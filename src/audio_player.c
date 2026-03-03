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
#include "audio_dac_registers.h"

// internal \\

//register writing methods
static int dac_reg_write(uint8_t reg, uint8_t val){
	uint8_t buffer[2] = {reg,val};
	return i2c_write_blocking(I2C_PORT,DAC_ADDR, buffer, sizeof(buffer), false);
}

static int dac_set_page(uint8_t page){
	return dac_reg_write(0x00,page);
}

static int dac_set_reg(uint8_t reg){
	uint8_t buffer[1] = {reg};
	return i2c_write_blocking(I2C_PORT,DAC_ADDR, buffer, sizeof(buffer), true);
}

static int dac_read_register(uint8_t page, uint8_t reg ){
	dac_set_page(page);
	dac_set_reg(reg);

	uint8_t value;
	i2c_read_blocking_until(I2C_PORT,DAC_ADDR,&value,sizeof(value),false,1000);
	
	return value;
}

static int set_bits(uint8_t reg, uint8_t mask, uint8_t shift,uint value){
	uint8_t reg_value = dac_read_register(DAC_REG_PG0,reg);
        reg_value &= ~(mask << shift);
        reg_value |= (value & mask) << shift;
        return dac_reg_write(reg, reg_value);
}

//dac methods
static bool dac_addr_response(void){
	uint8_t dummy;
	int result = i2c_read_blocking(I2C_PORT,DAC_ADDR,&dummy,1,false);
	return (result >= 0);
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
			panic("error writing to dac_volume - dac_mute() mute \n");
			return -1;
		}
	}
	else{ //unmute
		if (dac_reg_write(DAC_VOLUME_CONTROL,0x00) == PICO_ERROR_GENERIC){
			panic("error writing to dac_volume - dac_mute() unmute \n");
			return -1;
		}
	}
	return 0;
}

// dac init
static void dac_i2c_start(void){
#if !defined(I2C_PORT) || !defined(DAC_SDA_GPIO_PIN) || !defined(DAC_SCL_GPIO_PIN)
	#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined\n");
#else
	i2c_init(I2C_PORT,400*1000);

	gpio_set_function(DAC_SDA_GPIO_PIN,GPIO_FUNC_I2C);
	gpio_set_function(DAC_SCL_GPIO_PIN,GPIO_FUNC_I2C);

	gpio_pull_up(DAC_SDA_GPIO_PIN);
	gpio_pull_up(DAC_SCL_GPIO_PIN);
	
#endif
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
	sleep_ms(10);
}

static int dac_init(void){
	//set page 0
	if (dac_set_page(DAC_REG_PG0) == PICO_ERROR_GENERIC){
		panic("reg - 0x00 set page 0\n" );
		return -1;
	}

	//reset registers
	if (dac_reg_write(0x01,0x01) == PICO_ERROR_GENERIC){
		panic("reg - 0x01 reset dac software\n");
		return -1;
	}
	sleep_ms(10);

	return 0;
}

static int dac_configure_clocks(void){
	//ensure DAC and PLL are powered down
	set_bits(DAC_DATA_PATH_REG,0x03,6,0b00);
	set_bits(DAC_PLL_PROG_PR,0x01,7,0b0);
	sleep_ms(10);

	//set PLL clock scaling registers
	dac_reg_write(DAC_PLL_PROG_PR,81);
	dac_reg_write(DAC_PLL_PROG_J,35);
	dac_reg_write(DAC_PLL_PROG_D_MSB,29);
	dac_reg_write(DAC_PLL_PROG_D_LSB,80);

	//set mux for PLL input clock source (PLL_CLKIN)
	set_bits(DAC_CLOCK_MUX1,0x0c,2,DAC_CLOCK_MUX1_SOURCE);

	//power up PLL and wait briefly for PLL lock
	set_bits(DAC_PLL_PROG_PR, 0x01, 7, 0b1);
	sleep_ms(10);

	//set mux to route PLL output (PLL_CLK) to CODEC_CLKIN
	set_bits(DAC_CLOCK_MUX1,0x03,0,0b11);

	//set data format I2S
	dac_reg_write(DAC_CODEC_IF,0);

	//configure codec clock dividers for oversamply and dsp (might not be needed?)
	dac_reg_write(DAC_NDAC_REG,147);
	dac_reg_write(DAC_MDAC_REG,129);
	dac_reg_write(DAC_DOSR_MSB_REG,0);
	dac_reg_write(DAC_DOSR_LSB_REG,128);

	//power up dac
	set_bits(DAC_DATA_PATH_REG, 0x03, 6, 0b11);
	
	return 0;
}

static int dac_configure_headphones(void){
	//set volumes low
	if (set_channel_volume(true,-63.5) == PICO_ERROR_GENERIC){ //right
		panic("reg - 0x01 set volumes low R\n" );
		return -1;
	}
	if (set_channel_volume(false,-63.5) == PICO_ERROR_GENERIC){ //right
		panic("reg - 0x01 set volumes low L\n" );
		return -1;
	}

	//configure dac output
	//L + R enabled & I2S enabled
	dac_reg_write(DAC_DATA_PATH_REG,DAC_DATA_PATH_VALUE);

	//set volume control
	dac_reg_write(DAC_VOLUME_CONTROL,0x00);


	dac_set_page(DAC_REG_PG1);
	dac_reg_write(DAC_HEADPHONES_DRIVER_REG, DAC_HEADPHONES_DRIVER_VAL);
	dac_set_page(DAC_REG_PG0);

	return 0;
}

static int dac_register_setup(void){
	dac_init();
	dac_configure_clocks();
	dac_configure_headphones();

/*	//route DAC to output
	if (dac_reg_write(0x3F,0xD4) == PICO_ERROR_GENERIC){
		panic("error - writing to reg 0x3F dac_reg_write() route DAC to output \n");
		return -1;
	}
		*/

	//unmute
	if (dac_mute(false) == -1){
		return -1;
	}

	printf("finished reg setup\n");
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
