//pico libraries
#include "pico/stdlib.h"
//project libraries
#include "led_manager.h"

//
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h" 
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

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

int led_init(void){
	return Pico_led_init();
}

void blink(void){
	int rc = Pico_led_init();
	hard_assert(rc == PICO_OK);

	while(true){
		Pico_set_led(true);
		sleep_ms(250);
		Pico_set_led(false);
		sleep_ms(250);
	}

}
