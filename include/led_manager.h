#ifndef LED_DELAY_MS
#define LED_DELAY_MS 250
#endif

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h" 
#endif

#ifndef LED_MANAGER_H
#define LED_MANAGER_H

void ldm_led_init(void);
void ldm_led_set(bool led_on);
void ldm_led_blink(void);

#endif
