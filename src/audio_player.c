//c libraries
#include <stdbool.h>
#include <stdio.h>

//project libraries
#include "audio_player.h"
#include "audio_dac_bridge.h"


// internal \\

//public

void audio_init(void){
	DAC_i2c_wakeup();
}

void audio_close(void){
	// need to implement
	DAC_powerdown();
}

void play_noise(void){
	// need to implement

}
