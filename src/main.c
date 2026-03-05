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
	set_sys_clock_khz(132000,true); //132KHz / 11 = 12 MHz
	stdio_init_all();

	//led
	led_blink();

	sleep_ms(1500);
	printf("hello father\n");
	//test mclk
	gpio_init(DAC_MCLK_GPIO_PIN);
	gpio_set_dir(DAC_MCLK_GPIO_PIN,GPIO_IN);
	int last = -1;
	int transitions = 0;
	for (int i = 0; i < 1000000;i++){
		int val = gpio_get(15);
		if (val != last){
			transitions++;
			last = val;
		}
	}
	printf("MCLK transitions: %d (expect many thousands)\n", transitions);

	printf("audio_init\n");
	audio_init();
	printf("MCLK PWM slice: %d\n",pwm_gpio_to_slice_num(DAC_MCLK_GPIO_PIN));
	printf("MCLK PWM channel: %d\n",pwm_gpio_to_channel(DAC_MCLK_GPIO_PIN));

	printf("system clock: %lu Hz\n",clock_get_hz(clk_sys));

	sleep_ms(10);
	uint slice = pwm_gpio_to_slice_num(DAC_MCLK_GPIO_PIN);
	printf("PWM counter: %d\n", pwm_get_counter(slice));
	//alive message
	struct repeating_timer alive_message_timer;
	add_repeating_timer_ms(-5000,aliveMessage,NULL,&alive_message_timer);




	//infinite loop
	for(;;);
	return 0;
}
