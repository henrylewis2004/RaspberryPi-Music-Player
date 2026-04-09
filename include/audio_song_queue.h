#ifndef AUDIO_SONG_QUEUE_H
#define AUDIO_SONG_QUEUE_H

#ifndef MAX_SONG_QUEUE_LENGTH 
#define MAX_SONG_QUEUE_LENGTH 50
#endif

#ifndef MAX_SONG_PATH_LENGTH //NOTE: might need to make conistent accross systems
#define MAX_SONG_PATH_LENGTH 128
#endif

typedef struct {
	char* path[MAX_SONG_PATH_LENGTH];

	song_queue* previous_song; //NOTE: maybe include at a later date?
	song_queue* next_song;

	uint song_index;
} song_queue;

typedef const uint song_queue_size_t = MAX_SONG_PATH_LENGTH + 16 + sizeof(uint);

#endif // !DEBUG
