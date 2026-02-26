// C stdlib headers
#include <stdlib.h>
// FatFS headers
#include "f_util.h"
#include "ff.h"
//project headers
#include "sd_memory_manager.h"
#include "hw_config.h"


// internal \\

static FATFS file_system;

static int sd_mount(void){
	FRESULT fr = f_mount(&file_system,"",1);
	if (fr != FR_OK){
		panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
		return -1;
	}
	return 0;
}

static void sd_unmount(void){
	f_unmount("");
}

static int sd_create_file(char* filepath,bool overwrite){
	FIL file;
	FRESULT fr;

	//open file
	if (overwrite){
		fr = f_open(&file, filepath, FA_CREATE_ALWAYS);
	}
	else{
		fr = f_open(&file, filepath, FA_CREATE_NEW);
	}

	if (fr != FR_OK && fr != FR_EXIST){
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return -1;
	}

	f_close(&file);

	return 0;
}

static char* sd_read_file(char* filepath){
	FIL file;
	FRESULT fr = f_open(&file, filepath, FA_READ);

	printf("- read test -\n");
	//open file
	if (fr != FR_OK && fr != FR_EXIST){
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return NULL;
	}
	printf("read test - opened file\n");

	//read file
	char* file_buffer = malloc(MAX_FILE_SIZE);
	uint bytes_read;

	fr = f_read(&file, file_buffer,MAX_FILE_SIZE,&bytes_read);
	printf("read test - read file\n");
	if (fr != FR_OK && fr != FR_EXIST){
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return NULL;
	}
	f_close(&file);
	printf("read test - closed file\n");

	return file_buffer;
}

static int sd_write_file(char* filepath, char* data_in,size_t input_size, bool write_append){
	FIL file;
	FRESULT fr;
	uint bytes_written;


	//open file
	if (write_append){
		fr = f_open(&file, filepath, FA_OPEN_APPEND | FA_WRITE);
	}
	else{
		fr = f_open(&file, filepath, FA_OPEN_ALWAYS | FA_WRITE);
	}

	if (fr != FR_OK && fr != FR_EXIST){
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return -1;
	}

	//write to file
	if (f_write(&file, data_in, input_size, &bytes_written) < 0){
		panic("f_printf failed\n");
		return -1;
	}

	f_close(&file);

	return 0;
}

// public \\

void sd_init(void){
	sd_mount();
}

void sd_close(void){
	sd_unmount();
}

void sd_functionality_test(void){
	printf("sd_mount\n");
	sd_mount();

	printf("read test\n");
	char* const filepath1 = "test.txt";
	printf("read_test: %s\n", sd_read_file(filepath1));

	printf("create test\n");
	char* const filepath_create = "create_test";
	sd_create_file(filepath_create,true);

	printf("write test\n");
	sd_write_file(filepath_create,"hello world!\n",14, true);

	sd_unmount();
}
