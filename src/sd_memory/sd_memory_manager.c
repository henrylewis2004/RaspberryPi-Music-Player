// C stdlib headers
#include <stdlib.h>
// FatFS headers
#include "f_util.h"
#include "ff.h"
//project headers
#include "sd_memory_manager.h"
#include "hw_config.h"


#ifndef WAV_FILE
#define WAV_FILE

typedef struct{
	struct header{
		char id[4];
		uint32_t fileSize; //file size - 8 bytes
		char fileFormat[4];

	};

	struct format{
		char chunkId[4];
		uint32_t chunkSize;
		uint16_t audioFormat;
		uint16_t numChannels;
		uint32_t sampleRate;
		uint32_t byteRate;
		uint16_t blockAlign;
		uint16_t bitsPerSample;
	};

	struct data {
		char chunkId[4];
		uint32_t chunkSize;
		// add data pointer 
	};

} wav_file;


#endif

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
		f_close(&file);
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return -1;
	}

	f_close(&file);

	return 0;
}

static char* sd_read_file(char* filepath, uint file_size){
	FIL file;
	FRESULT fr = f_open(&file, filepath, FA_READ);

	//open file
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(&file);
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return NULL;
	}

	//read file
	char* file_buffer = malloc(file_size);
	if (file_buffer == NULL){
		f_close(&file);
		panic("sd_read error: failed to create file_buffer\n");
		return NULL;
	}

	uint bytes_read;

	fr = f_read(&file, file_buffer,file_size,&bytes_read);

	if (fr != FR_OK && fr != FR_EXIST){
		f_close(&file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return NULL;
	}
	f_close(&file);

	file_buffer[bytes_read] = '\0';

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
		f_close(&file);
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
		return -1;
	}

	//write to file
	fr = f_write(&file, data_in, input_size, &bytes_written);
	if (fr != FR_OK || bytes_written < input_size){ 
		f_close(&file);
		panic("f_printf failed\n");
		return -1;
	}

	f_close(&file);

	return 0;
}

// wav
int read_wav(void){
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
	char* read_result = sd_read_file(filepath1,MAX_FILE_SIZE);
	printf("%s: %s\n",filepath1, read_result);

	free(read_result);
	read_result = NULL;

	printf("create test\n");
	char* const filepath_create = "create_test";
	sd_create_file(filepath_create,true);

	printf("write test\n");
	sd_write_file(filepath_create,"hello world!\n",14, true);

	printf("read + write test\n");
	char* const filepath_create2 = "create_test2";
	read_result = sd_read_file(filepath1,MAX_FILE_SIZE);
	sd_write_file(filepath_create,read_result,MAX_FILE_SIZE, false);

	free(read_result);
	read_result = NULL;

	sd_unmount();
	printf("sd tests finished\n");
}
