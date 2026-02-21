//standard C libraries
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"
//#include "pico/time.h"
//project libraries
#include "led_manager.h"
#include "audio_player.h"

static bool aliveMessage(struct repeating_timer *t){
	printf("I Breathe Father\n");
	return true;
}

// main \\

int main(void){
	stdio_init_all();

	//led
	led_blink();

	//alive message
	struct repeating_timer timer;
	add_repeating_timer_ms(-1000,aliveMessage,NULL,&timer);


	//infinite loop
	for(;;);
	return 0;
}
