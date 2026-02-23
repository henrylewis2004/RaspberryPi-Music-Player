// FatFS headers
#include "f_util.h"
#include "ff.h"
//project headers
#include "sd_memory_manager.h"
#include "hw_config.h"


// internal \\

/* 
 
 steps:
 1. find sd
 2. mount sd
 3. read sd
 4. store data
 5. retrieve data


 */

int sd_mount(void){

	FATFS fs; //FatFs filesystem
	FRESULT fr = f_mount(&fs,"",1);
	if (fr != FR_OK){
		panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
		return -1;
	}

	FIL fil; //file object
	const char* const filename = "sd_text_write.txt";
	fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);

	if (fr != FR_OK && fr != FR_EXIST){
		panic("f_open(%s) error: %s (%d)\n",filename, FRESULT_str(fr), fr);
		return -1;
	}

	if (f_printf(&fil, "hello world!\n") < 0){
		panic("f_printf failed\n");
		return -1;
	}

	fr = f_close(&fil);
	if (fr != FR_OK){
		panic("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
		return -1;
	}

	f_unmount("");
	printf("goodbye!");
}



// public \\
