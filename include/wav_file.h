#ifndef WAV_FILE_H
#define WAV_FILE_H

typedef struct {
	char id[4];
	uint32_t chunkSize;
	char format[4];

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
} fmt_sub_chunk;

typedef struct {
	char chunkId[4];
	uint32_t chunkSize;
} data_sub_chunk;

typedef struct{
	wav_header header;
	fmt_sub_chunk fmt_chunk;
	data_sub_chunk data;
} wav_file;

#endif
