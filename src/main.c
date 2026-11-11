#include "controller.h"

// main \\

int main(void){
	controller_system_init();

	//controller_song_test(true);
	controller_system_loop();
}
