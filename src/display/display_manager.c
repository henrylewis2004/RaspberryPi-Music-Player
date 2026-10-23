//pico headers
#include "pico/stdlib.h"

//project headers
#include "display_manager.h"
#include "display_hardware_bridge.h"
#include "display_oled_pin_values.h"

// public \\

void display_init(void){
	display_hardware_init();
}

void display_test_run(void){
	display_hardware_draw_test();
}
