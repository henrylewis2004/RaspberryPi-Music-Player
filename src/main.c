//standard C headers
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "my_debug.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

//project headers
#include "led_manager.h"
#include "audio_player.h"
#include "sd_memory_manager.h"
#include "audio_dac_values.h"

static bool aliveMessage(struct repeating_timer *t){
	printf("I Breathe Father\n");
	return true;
}

// main \\

int main(void){
	set_sys_clock_khz(150000,true); //132KHz / 11 = 12 MHz
	stdio_init_all();
	//led
	led_blink();

	sleep_ms(1500);
	printf("hello father\n");



	printf("audio_init\n");
	audio_init();

	//alive message
	struct repeating_timer alive_message_timer;
	add_repeating_timer_ms(-5000,aliveMessage,NULL,&alive_message_timer);




	//infinite loop
	for(;;);
	return 0;
}
