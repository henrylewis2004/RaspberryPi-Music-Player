//c headers
#include <stdio.h>
//pico headers
#include "pico/stdlib.h"
//project headers
#include "input.h"

static input_flags input_button_flags;

// internal \\

void button_playback(uint gpio, uint32_t events){
	printf("button pressed: ");
	if (gpio == PLAY_BUTTON_PIN){
		printf("play button pressed\n");
		input_button_flags.play_button = true;
	}

	return;
}

void button_init(int gpio_pin){
	gpio_init(gpio_pin);
	gpio_set_dir(gpio_pin,GPIO_IN);
	gpio_pull_up(gpio_pin);

	gpio_set_irq_enabled_with_callback(gpio_pin,GPIO_IRQ_EDGE_FALL,true,&button_playback);
}

void reset_play_button_flag(void){
	input_button_flags.play_button = false;
}

// public \\

void input_init(void){
	button_init(PLAY_BUTTON_PIN);

	input_reset_flags();
}

void input_reset_flags(void){
	reset_play_button_flag();
}

input_flags input_get_flags(void){
	return input_button_flags;
}

input_flags input_get_reset_flags(void){
	input_reset_flags();
	return input_button_flags;
}
