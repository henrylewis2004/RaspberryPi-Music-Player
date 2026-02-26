//c libraries
#include <stdbool.h>
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#if PICO_ON_DEVICE
#include "hardware/clocks.h"
#include "hardware/structs/clocks.h"
#endif

#if USE_AUDIO_I2S
#include "pico/audio_i2s.h"
#endif

//project libraries
#include "audio_player.h"
#include "audio_dac_pins.h"





//internal
static int dac_reg_write(uint8_t reg, uint8_t val){
	uint8_t buffer[2] = {reg,val};
	return i2c_write_blocking(I2C_PORT,DAC_ADDR, buffer, 2, false);
}

static void dac_i2c_start(void){
#if !defined(I2C_PORT) || !defined(DAC_SDA_GPIO_PIN) || !defined(DAC_SCL_GPIO_PIN)
	#warning i2c/bus_scan example requires a board with I2C pins
    puts("Default I2C pins were not defined\n");
#else

	printf("dac start i2c pins\n");
	i2c_init(i2c_default,10*1000);

	gpio_set_function(DAC_SDA_GPIO_PIN,GPIO_FUNC_I2C);
	gpio_set_function(DAC_SCL_GPIO_PIN,GPIO_FUNC_I2C);

	gpio_pull_up(DAC_SDA_GPIO_PIN);
	gpio_pull_up(DAC_SCL_GPIO_PIN);
	
	bi_decl(bi_2pins_with_func(DAC_SDA_GPIO_PIN,DAC_SCL_GPIO_PIN,GPIO_FUNC_I2C))
#endif
}

static bool dac_addr_response(void){
	uint8_t dummy;
	int result = i2c_read_blocking(I2C_PORT,DAC_ADDR,&dummy,1,false);
	printf("dac response result: %d\n", result);
	return (result >= 0);
}

static void dac_reset(void){
	printf("dac reset pin\n");
	//reset hardware - pullup reset pin
	//setup reset pin
	gpio_init(DAC_RESET_GPIO_PIN);
	gpio_set_dir(DAC_RESET_GPIO_PIN,GPIO_OUT);

	//reset dac
	gpio_put(DAC_RESET_GPIO_PIN,false);
	sleep_ms(100);
	gpio_put(DAC_RESET_GPIO_PIN,true);
}

int dac_register_setup(void){
	printf("dac reg setup\n");
	printf("dac reg setup: write 0x00 0x00 \n");
	int res = dac_reg_write(0x00,0x00); //set page 0
	printf("result: %d\n", res);
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x00 error:(%d)\n", res);
		return -1;
	}

	res = dac_reg_write(0x01,0x01); //reset
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x01 error:(%d)\n", res);
		return -1;
	}
	sleep_ms(10);

	printf("dac reg setup: write 0x04 0x03 \n");
	res = dac_reg_write(0x04,0x03); //set clock to BCK
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x04 error:(%d)\n", res);
		return -1;
	}
	printf("dac reg setup: write 0x1B 0x0c \n");
	res = dac_reg_write(0x1B,0x0c); //set I2S, 16 bit, slave
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x1B error:(%d)\n", res);
		return -1;
	}
	printf("dac reg setup: write 0x3F 0xD4 \n");
	res = dac_reg_write(0x3F,0xD4); //route DAC to output
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x3F error:(%d)\n", res);
		return -1;
	}
	printf("dac reg setup: write 0x40 0x00 \n");
	res = dac_reg_write(0x40,0x00); //unmute
	if (res==PICO_ERROR_GENERIC){
		panic("reg - 0x40 error:(%d)\n", res);
		return -1;
	}
	printf("finished reg setup\n");
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

	i2c_bus_scan();


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
