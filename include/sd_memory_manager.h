#ifndef SD_MEMORY_MANAGER_H
#define SD_MEMORY_MANAGER_H

#ifndef MAX_FILE_SIZE
#define MAX_FILE_SIZE 4096
#endif


void sd_init(void);
void sd_close(void);
void sd_functionality_test(void);

void sd_set_playsong(char* filepath);
bool sd_wav_read_data(uint32_t* buf);
void sd_wav_close_playing_song(void); 


#endif // !SD_MEMORY_MANAGER_H
