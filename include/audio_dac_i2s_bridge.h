#ifndef AUDIO_DAC_I2S_BRIDGE_H
#define AUDIO_DAC_I2S_BRIDGE_H

#define I2S_BUFFER_WORDS 512

typedef void (*i2s_buffer_callback_t)(uint32_t *buffer, size_t word_count);

void DAC_i2s_init(i2s_buffer_callback_t callback);

void DAC_set_callback(i2s_buffer_callback_t callback);

static inline uint32_t i2s_pack_sample(int16_t left, int16_t right){
	return ((uint32_t)(uint16_t)left << 16) | (uint16_t)right;
}

#endif
