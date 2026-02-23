//standard C headers
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "my_debug.h"

//project headers
#include "led_manager.h"
//#include "audio_player.h"
#include "sd_memory_manager.h"

static bool aliveMessage(struct repeating_timer *t){
	printf("I Breathe Father\n");
	return true;
}

// main \\

int main(void){
	stdio_init_all();

	//led
	led_blink();

	sleep_ms(1500);
	printf("hello father\n");

	sd_mount();


	//alive message
	struct repeating_timer alive_message_timer;
	add_repeating_timer_ms(-5000,aliveMessage,NULL,&alive_message_timer);


	//infinite loop
	for(;;);
	return 0;
}
