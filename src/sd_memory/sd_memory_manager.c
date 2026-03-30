// C stdlib headers
#include <stdlib.h>
#include <stdbool.h>
// FatFS headers
#include "f_util.h"
#include "ff.h"
//project headers
#include "pico/stdlib.h"
#include "sd_memory_manager.h"
#include "hw_config.h"
#include "wav_file.h"
#include "audio_dac_i2s_values.h"

// internal \\

#ifndef BUFFER_REFIL_CNT
#define BUFFER_REFIL_CNT 1
#endif /* ifndef BUFFER_REFIL_CNT */


typedef struct {
	FIL file;
	uint32_t data_byte_length;
	uint32_t bytes_read_total;

	uint32_t buffer_refil[BUFFER_REFIL_CNT][I2S_BUFFER_WORDS];
	uint curBuffer;
	wav_file wfile;
} playing_song;

playing_song* curSongPtr;

static FATFS file_system;

//file system methods
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
	}

	//read file
	char* file_buffer = malloc(file_size);
	if (file_buffer == NULL){ f_close(&file);
		panic("sd_read error: failed to create file_buffer\n");
	}

	uint bytes_read;

	fr = f_read(&file, file_buffer,file_size,&bytes_read);

	if (fr != FR_OK && fr != FR_EXIST){
		f_close(&file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
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
static wav_file read_wav(char* filepath, FIL* file){
	FRESULT fr = f_open(file, filepath, FA_READ);

	//open file
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_open(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	//read file
	wav_file wfile;
	uint bytes_read;

	//header
	fr = f_read(file,wfile.header.id,4,&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) wav header id error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,&wfile.header.fileSize,sizeof(uint32_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) wav header file size error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,wfile.header.fileFormat,4,&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) wav header file format error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	//format
	fr = f_read(file,wfile.format.chunkId,4,&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) wav format id error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,&wfile.format.chunkSize,sizeof(uint32_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) wav format chunk size error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,&wfile.format.audioFormat,sizeof(uint16_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	fr = f_read(file,&wfile.format.numChannels,sizeof(uint16_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	fr = f_read(file,&wfile.format.sampleRate,sizeof(uint32_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,&wfile.format.byteRate,sizeof(uint32_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	fr = f_read(file,&wfile.format.blockAlign,sizeof(uint16_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	fr = f_read(file,&wfile.format.bitsPerSample,sizeof(uint16_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}


	//data
	fr = f_read(file,wfile.data.chunkId,4,&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}
	fr = f_read(file,&wfile.data.chunkSize,sizeof(uint32_t),&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(file);
		panic("f_read(%s) error: %s (%d)\n",filepath, FRESULT_str(fr), fr);
	}

	//need to change to malloc (if data is needed)
	return wfile;
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

uint sd_get_cur_buffer(void){
	return curSongPtr->curBuffer;
}

uint sd_get_next_buffer(void){
	if (sd_get_cur_buffer() + 1 + 1 > BUFFER_REFIL_CNT){
		return 0;
	}
	return sd_get_cur_buffer() + 1;
}


uint32_t* sd_get_next_samples(void){
	uint temp_buf = curSongPtr->curBuffer;
	curSongPtr->curBuffer = sd_get_next_buffer();

	return curSongPtr->buffer_refil[temp_buf];
}

void sd_set_playsong(char* filepath){
	curSongPtr->wfile = read_wav(filepath, &curSongPtr->file);
	curSongPtr->bytes_read_total = 0;
	for (int i = 0; i < BUFFER_REFIL_CNT; i++){
		sd_wav_read_data(i);
	}
}

bool sd_wav_read_data(uint buf){
	uint32_t read_size = I2S_BUFFER_WORDS * sizeof(uint32_t);

	if (curSongPtr->data_byte_length - curSongPtr->bytes_read_total < read_size){
		read_size = curSongPtr->data_byte_length - curSongPtr->bytes_read_total;
	}


	uint bytes_read;
	FRESULT fr = f_read(&curSongPtr->file,curSongPtr->buffer_refil[buf],read_size,&bytes_read);
	if (fr != FR_OK && fr != FR_EXIST){
		f_close(&curSongPtr->file);
		panic("f_read error: %s (%d)\n", FRESULT_str(fr), fr);
		return false;
	}
	curSongPtr->bytes_read_total += bytes_read;

	if (curSongPtr->bytes_read_total >= curSongPtr->data_byte_length){
		return true; //stream finished
	}

	return false;
}


//TODO: add checing 
void sd_wav_close_playing_song(void){
	f_close(&curSongPtr->file);
}
