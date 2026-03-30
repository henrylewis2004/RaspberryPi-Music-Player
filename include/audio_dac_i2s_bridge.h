#ifndef AUDIO_DAC_I2S_BRIDGE_H
#define AUDIO_DAC_I2S_BRIDGE_H

typedef void (*buffer_callback_t)(uint32_t*);

void i2s_set_buffer_callback(buffer_callback_t buffer_callback);
void DAC_i2s_init(buffer_callback_t buffer_callback);
void DAC_start_dma(void);
void DAC_stop_dma(void);

buffer_callback_t i2s_get_buffer_callback_function(void);

#endif
