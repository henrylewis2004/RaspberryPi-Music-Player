#ifndef WAV_FILE
#define WAV_FILE

typedef struct {
	char id[4];
	uint32_t fileSize; //file size - 8 bytes
	char fileFormat[4];

} wav_header;

typedef struct {
	char chunkId[4];
	uint32_t chunkSize;
	uint16_t audioFormat;
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
} wav_format;

typedef struct {
	char chunkId[4];
	uint32_t chunkSize;
	// add data pointer 
} wav_data ;

typedef struct{
	wav_header header;
	wav_format format;
	wav_data data;
} wav_file;


#endif
