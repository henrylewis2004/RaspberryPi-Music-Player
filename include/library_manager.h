#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

//frame 
#ifndef MAX_FRAME_COUNT
#define MAX_FRAME_COUNT 25
#endif

#ifndef NEXT_FRAME_LOAD_COUNT
#define NEXT_FRAME_LOAD_COUNT 5
#endif

//artist
#ifndef MAX_ARTIST_NAME_LENGTH 
#define MAX_ARTIST_NAME_LENGTH 128
#endif

//album
#ifndef MAX_ALBUM_NAME_LENGTH 
#define MAX_ALBUM_NAME_LENGTH 128
#endif

//song
#ifndef MAX_SONG_NAME_LENGTH
#define MAX_SONG_NAME_LENGTH 128
#endif

#ifndef MAX_SONG_PATH_LENGTH //NOTE: might need to make conistent accross systems
#define MAX_SONG_PATH_LENGTH 128
#endif

//info structs
typedef struct {
	char name[MAX_SONG_NAME_LENGTH];
	char filepath[MAX_SONG_PATH_LENGTH];
	uint32_t length; //seconds

} song_info;


typedef struct {
	char name[MAX_ALBUM_NAME_LENGTH];
	song_info *song_list; 
	size_t song_count;
	// maybe add year of album?

} album_info;

typedef struct {
	char name[MAX_ARTIST_NAME_LENGTH];
	album_name *album_list;
	size_t album_count;

} artist_info;

//object structs
typedef struct {
	song_info *song_i;
	album_info *album_i;

} song;

typedef struct {
	album_info *album_i;
	artist_info *artist_i;

} album;


typedef struct {
	artist_info *artist_list;
	size_t artist_count;

} catalogue;

#endif
