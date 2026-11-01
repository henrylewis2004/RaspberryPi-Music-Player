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

#ifndef MAX_ARTIST_COUNT
#define MAX_ARTIST_COUNT 10
#endif

//info structs
typedef struct {

	//example tag
	/*LISTî: might be a number? might be unrelated
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

	uint track_number;

	uint year;
	uint month;
	uint date;

	/*
	char *song_writers[];
	char *producers[];	
	char country[4];
	char medium[4];
	*/

	char *artists[MAX_ARTIST_COUNT]; 
	char album_name[MAX_SONG_NAME_LENGTH];
	char song_title[MAX_SONG_NAME_LENGTH];

//	uint32_t length; //seconds //not a part of the tags might have to calculate seperately

} track_tag_info;

typedef struct {
	char name[MAX_SONG_NAME_LENGTH];
	char filepath[MAX_SONG_PATH_LENGTH];
	//uint track_number;
	uint length; //seconds

} song_info;


typedef struct {
	char name[MAX_ALBUM_NAME_LENGTH];
	uint year;
	song_info *song_list; 
	size_t song_count;

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
