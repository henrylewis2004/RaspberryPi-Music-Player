//c headers
#include <stdio.h>
//pico headers
#include "pico/stdlib.h"
//project headers
#include "input.h"


// internal \\

void button_playback(uint gpio, uint32_t events){
	printf("button pressed!\n");
}

void button_init(int gpio_pin){
	gpio_init(gpio_pin);
	gpio_set_dir(gpio_pin,GPIO_IN);
	gpio_pull_up(gpio_pin);

	gpio_set_irq_enabled_with_callback(gpio_pin,GPIO_IRQ_EDGE_FALL,true,&button_playback);
}

void play_button_init(void){
	button_init(PLAY_BUTTON_PIN);
}


// public \\

void input_init(void){
	play_button_init();
}
