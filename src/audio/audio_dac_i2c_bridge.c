//c libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/i2c.h" 
#include "hardware/pwm.h"
#include "hardware/clocks.h"

//project headers
#include "audio_dac_i2c_bridge.h"
#include "audio_dac_registers.h"
#include "audio_dac_pin_values.h"


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

static int dac_read_register(uint8_t reg ){
	//set reg
	dac_set_reg(reg);

	//read from register
	uint8_t value;
	if (i2c_read_blocking(I2C_PORT,DAC_ADDR,&value,sizeof(value),false) == PICO_ERROR_GENERIC){
		panic("error reading from register,  reg: %d\n",reg);
		return -1;
	}
	
	return value;
}

static int set_bits(uint8_t reg, uint8_t mask, uint8_t shift,uint value){
	uint8_t reg_value = dac_read_register(reg);
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
	//mute audio
	if (set_channel_volume(true,-63.5f) == PICO_ERROR_GENERIC){ //right
		panic("error muting dac in init\n");
		return -1;
	}
	if (set_channel_volume(false,-63.5f) == PICO_ERROR_GENERIC){ //right
		panic("error muting dac in init\n");
		return -1;
	}
	if (dac_mute(true) == PICO_ERROR_GENERIC){
		panic("error muting dac in init\n");
		return -1;
	}

	//set dac datapath
	dac_reg_write(DAC_DATA_PATH_REG,DAC_DATA_PATH_VALUE);
	dac_reg_write(DAC_VOLUME_CONTROL,0x00);

	return 0;
}

static int dac_configure_clocks(void){
	dac_set_page(DAC_REG_PG0);
	//ensure DAC and PLL are powered down
	set_bits(DAC_DATA_PATH_REG,0x03,6,0b00);
	set_bits(DAC_PLL_PROG_PR,0x01,7,0b0);
	sleep_ms(1);


	//set PLL clock scaling registers
	dac_reg_write(DAC_PLL_PROG_PR,81);
	dac_reg_write(DAC_PLL_PROG_J,35);
	dac_reg_write(DAC_PLL_PROG_D_MSB,29);
	dac_reg_write(DAC_PLL_PROG_D_LSB,80);

	//set mux for PLL input clock source (MCLK)
	set_bits(DAC_CLOCK_MUX1,0x0c,2,DAC_CLOCK_MUX1_SOURCE);

	//power up PLL and wait briefly for PLL lock
	set_bits(DAC_PLL_PROG_PR, 0x01, 7, 0b1);
	sleep_ms(10);


	//set mux to route PLL output (PLL_CLK) to CODEC_CLKIN
	set_bits(DAC_CLOCK_MUX1,0x03,0,0b11);

	//set data format I2S
	dac_reg_write(DAC_CODEC_IF,0xC0);

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
	if (ramp_set_dac_volume(-63.5,50,10) == PICO_ERROR_GENERIC){ 
		panic("reg - 0x01 set volumes ramp low\n" );
		return -1;
	}

	//configure dac output
	//L + R enabled & I2S enabled
	dac_reg_write(DAC_DATA_PATH_REG,DAC_DATA_PATH_VALUE);

	//set volume control
	dac_reg_write(DAC_VOLUME_CONTROL,0x00);

	//set page 1
	dac_set_page(DAC_REG_PG1);


	//HP drivers
	dac_reg_write(DAC_HPL_DRIVER, 0x07);
	dac_reg_write(DAC_HPR_DRIVER, 0x07);

	//HP gain
	dac_reg_write(DAC_HPL_VOL, 0x80); 
	dac_reg_write(DAC_HPR_VOL, 0x80);

	dac_reg_write(DAC_HEADPHONES_DRIVER_REG, DAC_HEADPHONES_DRIVER_VAL);

	//route dac output to HP drivers
	dac_reg_write(DAC_OUT_ROUTING_REG, DAC_OUT_ROUTING_VALUE);

	//set page 0
	dac_set_page(DAC_REG_PG0);
	dac_mute(false);
	ramp_set_dac_volume(-20.0f,50,10);

	return 0;
}

static int dac_register_setup(void){
	if (dac_init() == -1){
		panic("dac failed to init\n");
		return -1;
	}

	if (dac_configure_clocks() == -1){
		panic("dac failed to configure clocks\n");
		return -1;
	}

	if (dac_configure_headphones() == -1){
		panic("dac failed to configure headphones\n");
		return -1;
	}

	//unmute
	if (dac_mute(false) == -1){
		return -1;
	}

	// Read back DAC data path
	dac_set_page(DAC_REG_PG0);

	printf("finished reg setup\n");
	return 0;
}

static void confirm_register_setup(void){
	dac_set_page(DAC_REG_PG0);
	printf("PLL PR: 0x%02X (expect 0xD1)\n", dac_read_register(0x05));
	printf("PLL J: 0x%02X (expect 0x23)\n", dac_read_register(0x06));
	printf("PLL D MSB: 0x%02X (expect 0x1D)\n", dac_read_register(0x07));
	printf("PLL D LSB: 0x%02X (expect 0x50)\n", dac_read_register(0x08));
	printf("Clock mux: 0x%02X (expect bits 1:0=11, bits 3:2=00)\n", dac_read_register(0x04));
	printf("ndac: 0x%02X (expect 0x93)\n", dac_read_register(0x0B));
	printf("mdac: 0x%02X (expect 0x81)\n", dac_read_register(0x0C));


	printf("PLL status: 0x%02X (bit6 should be 1 if locked)\n", dac_read_register(0x26));
	printf("mux reg: 0x%02X\n", dac_read_register(0x04));
	printf("Data path: 0x%02X (expect 0xD4)\n", dac_read_register( 0x3F));
	// Read back CODEC interface
	printf("CODEC IF:  0x%02X (expect 0x00)\n", dac_read_register( 0x1B));
	// Read back page 1 routing
	dac_set_page(DAC_REG_PG1);
	printf("Route L & R:   0x%02X (expect 0x44)\n", dac_read_register( DAC_OUT_ROUTING_REG));
	printf("HPL gain:  0x%02X (expect 0x00)\n", dac_read_register( 0x10));
	printf("HPR gain:  0x%02X (expect 0x00)\n", dac_read_register( 0x11));
	printf("HP driver: 0x%02X (expect 0xC4)\n", dac_read_register( 0x1F));
	dac_set_page(DAC_REG_PG0);
	// Read back volume
	printf("Vol ctrl:  0x%02X (expect 0x00)\n", dac_read_register( 0x40));
	printf("Left vol:  0x%02X\n", dac_read_register( 0x41));
	printf("Right vol: 0x%02X\n", dac_read_register( 0x42));
}

//mclk generation from pico pwm
static int mclk_init(){
	gpio_set_function(DAC_MCLK_GPIO_PIN,GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(DAC_MCLK_GPIO_PIN);

	pwm_config pwm_cfg = pwm_get_default_config();
	pwm_config_set_wrap(&pwm_cfg, 9); 
	pwm_config_set_clkdiv(&pwm_cfg, 1);
	pwm_init(slice,&pwm_cfg,true);
	pwm_set_gpio_level(DAC_MCLK_GPIO_PIN,5); //50% duty

	return 0;
}

static float get_dac_channel_volume(bool right_channel){
	uint8_t reg = right_channel ? DAC_RIGHT_CHANNEL_REG : DAC_LEFT_CHANNEL_REG;

	//set page 0
	int8_t raw = (int8_t)dac_read_register(reg);
	return raw / 2.0f;

}

//public

float get_dac_volume(void){
	return get_dac_channel_volume(true); //assume that both channels will be the same volume
}

int dac_mute(bool mute){
	dac_set_page(0x00);
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

int ramp_set_dac_volume(float target_volume_db, uint steps, uint step_timer_ms){
	float initial_volume_db = get_dac_volume();
	float step = (target_volume_db - initial_volume_db) / steps;
	for (int i = 0; i <= steps; i++){
		float vol = initial_volume_db + step * i;
		if (set_channel_volume(true,vol) == PICO_ERROR_GENERIC){
			return -1;
		}
		if (set_channel_volume(false,vol) == PICO_ERROR_GENERIC){
			return -1;
		}
		sleep_ms(step_timer_ms);
	}
	return 0;
}

int set_channel_volume(bool right_channel, float volume_db){
	//right channel = 0x42, left channel = 0x41
	const uint8_t channel_register = (right_channel)? DAC_RIGHT_CHANNEL_REG : DAC_LEFT_CHANNEL_REG;
	dac_set_page(0x00);

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


//wakeup handshake
void DAC_i2c_wakeup(void){
	printf("dac wakeup\n");
	dac_i2c_start();
	mclk_init();
	dac_reset();
	
	if (dac_addr_response()){
		if (dac_register_setup() == -1){
			panic("error in dac register setup\n");
		}

	}
	else{
		printf("no DAC response to 0x%02X\n", DAC_ADDR);
	}
}

void DAC_powerdown(void){
}
