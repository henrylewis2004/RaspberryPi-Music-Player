//c libraries
#include <stdbool.h>
//pico libraries
#include "pico/stdlib.h"
#include "pico/time.h"
//project libraries
#include "led_manager.h"


#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h" 
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 500
#endif

// internal \\

static struct repeating_timer blink_timer;

//methods
static int Pico_led_init(void){
#if defined(PICO_DEFAULT_LED_PIN)
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
	return cyw43_arch_init();
#endif
}

static void Pico_set_led(bool led_on){
#if defined(PICO_DEFAULT_LED_PIN)
	gpio_put(PICO_DEFAULT_LED_PIN,led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,led_on);
#endif
}

static bool blink_callback(struct repeating_timer *t){
	static bool led_state = false;

	led_state = !led_state;
	Pico_set_led(led_state);

	return true;
}

// public \\

//methods
void led_init(void){
	Pico_led_init();
}

void led_set(bool led_on){
	Pico_set_led(led_on);
}


bool led_blink(void){
	led_init();
	return add_repeating_timer_ms(-500,blink_callback,NULL,&blink_timer);
}

void led_blink_stop(void){
	cancel_repeating_timer(&blink_timer);
}
