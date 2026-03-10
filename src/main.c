//standard C headers
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "my_debug.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"

//project headers
#include "led_manager.h"
#include "audio_player.h"
#include "sd_memory_manager.h"
#include "audio_dac_values.h"
//#include "input.h"

static bool aliveMessage(struct repeating_timer *t){
	printf("I Breathe Father\n");
	return true;
}

// main \\

int main(void){
	set_sys_clock_khz(150000,true); //150mhz / 10 = 15 MHz
	stdio_init_all();
	dma_claim_mask(0xfff);
	//led
	led_blink();

	sleep_ms(1500);
	printf("hello father\n");


	printf("audio_init\n");
	dma_unclaim_mask(0x30);
	audio_init();

	printf("sd_functionality_test\n");
	dma_unclaim_mask(0x0f);
	sd_init();




	//alive message
	struct repeating_timer alive_message_timer;
	add_repeating_timer_ms(-5000,aliveMessage,NULL,&alive_message_timer);




	//infinite loop
	for(;;);
	return 0;
}
