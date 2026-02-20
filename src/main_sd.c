//standard C libraries
#include <stdio.h>
//pico libraries
#include "pico/stdlib.h"
//sdio libaries
#include "f_util.h"
#include "ff.h"
#include "hw_config.h" //modified for specific use

//project libraries
#include "led_manager.h"

static sd_sdio_if_t sdio_if ={
	.CMD_gpio = 3,
	.D0_gpio = 4,
	.baud_rate = 125 * 1000 * 1000 * 6,

};

static sd_card_t sd_card = {.type = SD_IF_SDIO, .sdio_if_p = &sdio_if};

size_t sd_get_num() {return 1;}

sd_card_t* sd_get_by_num(size_t num){
	if (0 == num){
		return &sd_card;
	}
	else{
		return NULL;
	}
}

int readSD(char text[]) {
	FATFS fs;
	FRESULT fr = f_mount(&fs, "", 1);
	if (fr != FR_OK){
		strcpy(text, "f_mount error");
		return -1;
	}

	FIL fil;
	const char* const filename = "test.txt";
	fr = f_open(&fil,filename,FA_READ);

	if (fr != FR_OK) {
		strcpy(text, "f_open error");
		return -1;
	}
	else{
		int *bytesRead;
		fr = f_read(&fil,&text,sizeof(text),bytesRead);
	}

	fr = f_close(&fil);
	if (FR_OK != fr) {
		strcpy(text, "f_close error");
		return -1;
	}

	f_unmount("");
	return 0;

}

int main(){
	stdio_init_all();
	static char text[20] = "not loaded";
	readSD(text);


	puts("Goodbye, world!");
	for(;;){
		printf("%s", text);
		sleep_ms(500);
	}

}
