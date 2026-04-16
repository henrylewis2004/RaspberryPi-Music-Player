//c headers
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// project headers
#include "audio_song_queue.h"

//NOTE: might need to be volatile (when adding buttons?)
song_queue* top_song;
song_queue* last_song;

static uint8_t queue_length = 0;

// internal
static int add_song(char* filepath){
	if (queue_length > MAX_SONG_QUEUE_LENGTH){
		return -1;
	}

	queue_length++;

	if (top_song == NULL){
		top_song = calloc(1,sizeof(song_queue));
		strncpy(top_song->path, filepath, MAX_SONG_PATH_LENGTH);

		top_song->path[MAX_SONG_PATH_LENGTH - 1] = '\0';

		last_song = top_song;
		return 0;
	}

	song_queue* new_song = calloc(1,sizeof(song_queue));

	strncpy(new_song->path, filepath, MAX_SONG_PATH_LENGTH);
	new_song->path[MAX_SONG_PATH_LENGTH - 1] = '\0';

	new_song->previous_song = last_song;
	last_song->next_song = new_song;

	last_song = new_song;

	return 0;

}

static int remove_song_from_queue(uint8_t song_index){
	if (song_index >= MAX_SONG_QUEUE_LENGTH || song_index >= queue_length){
		return -1;
	}

	song_queue* cur_song = top_song;
	if (cur_song == NULL){
		return -1;
	}

	//top song
	if (song_index == 0){
		if (cur_song->next_song != NULL){
			song_queue* temp_top_ptr = top_song;
			top_song = cur_song->next_song;
			top_song->previous_song = NULL;

			free(temp_top_ptr);
		}
		else{
			free(top_song);
			top_song = NULL;
			last_song=NULL;
		}
		
		if (queue_length > 0){
			queue_length -= 1;
		}

		return 0;
	}

	//last song
	if (song_index == queue_length - 1){
		cur_song = last_song;
		if (cur_song->previous_song != NULL){
			song_queue* temp_last_ptr = cur_song;
			last_song = cur_song->previous_song;
			last_song->next_song = NULL;

			free(temp_last_ptr);
		}
		else{
			free(cur_song);
			last_song = NULL;
		}

		if (queue_length > 0){
			queue_length -= 1;
		}
		

		return 0;
	}

	//inbetween
	for (int i = 1; i <= song_index; i++){
		cur_song = cur_song->next_song;

		if (cur_song == NULL){
			return -1;
		}


		if (i == song_index){
			if (cur_song->previous_song != NULL){
				if (cur_song->next_song != NULL){
					cur_song->previous_song->next_song = cur_song->next_song;
					cur_song->next_song->previous_song = cur_song->previous_song;
				}
				else{
					cur_song->previous_song->next_song = NULL;
				}
			}


			free(cur_song);

			if (queue_length > 0){
				queue_length -= 1;
			}

			return 0;
		}

	}

	return -1; //song not found (incorrect index)
}


// public
int song_queue_add_song(char* filepath){
	return add_song(filepath);
}

int song_queue_remove_song_by_index(uint8_t index){
	return remove_song_from_queue(index);
}

int song_queue_goto_next_song(void){
	return remove_song_from_queue(0);
}

char* song_queue_get_next_song_path(void){
	if (top_song == NULL || top_song->next_song==NULL){
		return NULL;
	}
	return top_song->next_song->path;
}

bool song_queue_top_has_song(void){
	return (top_song != NULL);
}

char* song_queue_get_top_song_path(void){
	if (top_song == NULL || top_song->path ==NULL){
		return NULL;
	}
	return top_song->path;
}

int song_queue_clear(void){
	while(queue_length > 0){
		if (remove_song_from_queue(0) < 0){
			return -1;
		}
	}

	return 0;
}
