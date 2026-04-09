//c headers
#include <stdlib.h>

// project headers
#include "audio_song_queue.h"


volatile song_queue* top_song;
volatile song_queue* last_song;

volatile song_queue* queue_index_list[MAX_SONG_QUEUE_LENGTH];

// internal
static int new_song(char* filepath){
	if (top_song == NULL){
		top_song = malloc(song_queue_size_t);
		top_song->path = filepath;
		last_song = top_song;
		return 0;
	}

	song_queue* new_song = malloc(song_queue_size_t);
	new_song->path = filepath;
	new_song->previous_song = last_song;

	last_song->next_song = new_song;
	last_song = new_song;

}

static int remove_song_from_queue(uint song_index){
	song_queue* cur_song = top_song;
	for (int i = 0; i < song_index; i++){
		if (i == song_index){
			cur_song->previous_song->next_song = cur_song->next_song;
			free(cur_song);
			return 0;
		}

		cur_song = cur_song->next_song;
	}

	return -1; //song not found (incorrect index)
}


// public
int song_queue_add_song(char* filepath){
	new_song(filepath):
}

int queue_next_song(void){
	if (top_song == NULL){
		return -1;
	}
	song_queue* temp_song_ptr = top_song->next_song;
	free(top_song);

	top_song = temp_song_ptr;
	return 0;
}

char* get_next_song_path(void){
	return top_song->next_song.path;
}
