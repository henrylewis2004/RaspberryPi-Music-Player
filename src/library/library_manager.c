
#include "library_manager.h"
#include "sd_memory_manager.h"

//private

void add_entry_to_library(void){
}

track_tag_info get_title_tag_info(char *filepath){
	
}


//public

void library_sync(bool full_sync){
	//search sd and save catalogue
	
}

uint32_t library_get_artist_count(void){
	//placeholder
	return 1;
}

uint32_t library_get_artist_album_count(char* artist){
	//note: need to check artist is not null
	//placeholder
	return 1;
}

uint32_t library_get_album_song_count(char* artist, char* album){
	//note: need to check artist and album is not null
	//placeholder
	return 1;
}

