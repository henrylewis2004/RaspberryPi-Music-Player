//standard C libraries
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"

#include "led_manager.h"

//
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h" 
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

static int rc;

//
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

void ldm_led_init(void){
	rc = Pico_led_init();
	hard_assert(rc = PICO_OK);
}

void ldm_led_set(bool led_on){
	Pico_set_led(led_on);
}

void ldm_led_blink(void){
	if (rc != PICO_OK){
		ldm_led_init();
	}
	while (true){
		ldm_led_set(true);
		sleep_ms(LED_DELAY_MS);
		ldm_led_set(false);
		sleep_ms(LED_DELAY_MS);
	}
}




