//c headers
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// project headers
#include "audio_song_queue.h"

//NOTE: might need to be volatile (when adding buttons?)
song_queue* top_song;
song_queue* last_song;

uint8_t queue_length = 0;

// internal
static int add_song(char* filepath){
	if (queue_length >= MAX_SONG_QUEUE_LENGTH){
		return -1;
	}

	queue_length++;

	if (top_song == NULL){
		top_song = malloc(song_queue_size_t);
		strncpy(top_song->path, filepath, sizeof(filepath));

		if (sizeof(filepath) < MAX_SONG_PATH_LENGTH){
			top_song->path[sizeof(filepath)] = '\0';
		}

		last_song = top_song;
		return 0;
	}

	song_queue* new_song = malloc(song_queue_size_t);

	strncpy(new_song->path, filepath, sizeof(filepath));
	if (sizeof(filepath) < MAX_SONG_PATH_LENGTH){
		top_song->path[sizeof(filepath)] = '\0';
	}

	new_song->previous_song = last_song;

	last_song->next_song = new_song;
	last_song = new_song;

	return 0;

}

static int remove_song_from_queue(uint8_t song_index){
	if (song_index > MAX_SONG_QUEUE_LENGTH){
		return -1;
	}

	song_queue* cur_song = top_song;
	if (cur_song == NULL){
		return -1;
	}

	for (int i = 0; i <= song_index; i++){
		if (i == song_index){
			cur_song->previous_song->next_song = cur_song->next_song;
			cur_song->next_song->previous_song = cur_song->previous_song;
			free(cur_song);

			if (queue_length > 0){
				queue_length -= 1;
			}

			return 0;
		}

		cur_song = cur_song->next_song;

		if (cur_song == NULL){
			return -1;
		}
	}

	return -1; //song not found (incorrect index)
}


// public
int song_queue_add_song(char* filepath){
	add_song(filepath);
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
	return top_song->next_song->path;
}
