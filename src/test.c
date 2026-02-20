#include <stdio.h>
#include "pico/stdlib.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

int pico_led_init(void){
#if defined(PICO_DEFAULT_LED_PIN)
	gpio_init(PICO_DEFAULT_LED_PIN);
	gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
	return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
	return cyw43_arch_init();
#endif
}

void Pico_set_led(bool led_on){
#if defined(PICO_DEFAULT_LED_PIN)
	gpio_put(PICO_DEFAULT_LED_PIN,led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
	cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,led_on);
#endif

}


int main(){
	stdio_init_all();
	int rc = pico_led_init();
	hard_assert(rc==PICO_OK);
	while (true){
		printf("hello world");
		Pico_set_led(true);
		sleep_ms(LED_DELAY_MS);
		Pico_set_led(false);
		sleep_ms(LED_DELAY_MS);
	}

	return 0;
}
