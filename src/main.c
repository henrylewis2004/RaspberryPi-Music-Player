//standard C libraries
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"
//project libraries
#include "led_manager.h"

int main(){
	stdio_init_all();

	ldm_led_init();
	for(;;){
		ldm_led_set(true);
		sleep_ms(500);
		ldm_led_set(false);
		sleep_ms(500);
	}

}
