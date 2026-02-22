//c libraries
#include <stdbool.h>
//pico libraries
#include "pico/stdlib.h"
//project libraries
#include "audio_player.h"
#include "audio_dac_pins.h"





//internal
static void DAC_i2c_wakeup(){
	//wakeup handshake
	
	//reset hardware
	//pullup reset pin
	gpio_init(PICO_RESET_PIN);
	gpio_set_dir(PICO_RESET_PIN,GPIO_OUT);

	gpio_put(PICO_RESET_PIN,false);
	sleep_ms(20);
	gpio_put(PICO_RESET_PIN,true);
	sleep_ms(5);

	//
}

static void DAC_powerdown(){
}


//public
void audio_init(void){
	DAC_i2c_wakeup();
}

void play_noise(void){

}
