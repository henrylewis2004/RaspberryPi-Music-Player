#ifndef AUDIO_SONG_QUEUE_H
#define AUDIO_SONG_QUEUE_H

#ifndef MAX_SONG_QUEUE_LENGTH 
#define MAX_SONG_QUEUE_LENGTH 50
#endif

#ifndef MAX_SONG_PATH_LENGTH //NOTE: might need to make conistent accross systems
#define MAX_SONG_PATH_LENGTH 128
#endif

typedef struct song_queue song_queue;

struct song_queue {
	char path[MAX_SONG_PATH_LENGTH];

	song_queue* previous_song; //NOTE: maybe include at a later date?
	song_queue* next_song;

};

//methods
int song_queue_add_song(char* filepath);
int song_queue_remove_song_by_index(uint8_t index);

int song_queue_goto_next_song(void);
char* song_queue_get_next_song_path(void);
char* song_queue_get_top_song_path(void);
bool song_queue_top_has_song(void);

int song_queue_clear(void);

#endif // !DEBUG
