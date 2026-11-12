#ifndef SD_MEMORY_MANAGER_H
#define SD_MEMORY_MANAGER_H

#ifndef MAX_FILE_SIZE
#define MAX_FILE_SIZE 4096
#endif

//NOTE: might want to move values and struct to different header
#ifndef MAX_SONG_PATH_LENGTH
#define MAX_SONG_PATH_LENGTH 128
#endif

#ifndef MAX_SONG_NAME_LENGTH
#define MAX_SONG_NAME_LENGTH 128
#endif

#ifndef MAX_ARTIST_COUNT
#define MAX_ARTIST_COUNT 10
#endif

typedef struct {
	//example tag
	/* LIST: might be a number? might be unrelated
	 * INFO: info chunk
	 * ITRK: round the World
	 * ICRD:1999-06-08
	 * IENGG:Greg Collins, Greg Fidelman, David Schiffman, Jim Scott, John Sorenson
	 * IPRO:Rick Rubin
	 * ICNT:US
	 * IPRD:Californication
	 * IMED:CD
	 * IART:Red Hot Chili Peppers
	*/

	size_t chunk_size;
	char filepath[MAX_SONG_PATH_LENGTH];

	uint32_t track_number;

	uint32_t year;
	uint32_t month;
	uint32_t date;

	/*
	char *song_writers[];
	char *producers[];	
	char country[4];
	char medium[4];
	*/

	char *artists[MAX_ARTIST_COUNT]; 
	char album_name[MAX_SONG_NAME_LENGTH];
	char song_title[MAX_SONG_NAME_LENGTH];

	uint32_t length; //seconds //not a part of the tags might have to calculate seperately

} track_tag_info;

void sd_init(void);
void sd_close(void);
void sd_functionality_test(void);

void sd_set_playsong(char *filepath);
bool sd_wav_read_data(volatile uint32_t *buf);
void sd_wav_close_playing_song(void); 


#endif // !SD_MEMORY_MANAGER_H
