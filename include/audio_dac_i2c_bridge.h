#ifndef AUDIO_DAC_I2C_BRIDGE_H
#define AUDIO_DAC_I2C_BRIDGE_H

int dac_mute(bool mute);
int set_channel_volume(bool right_channel, float volume_db);
int ramp_set_dac_volume(float target_volume_db,uint steps, uint step_timer_ms);
float get_dac_volume(void);

void DAC_i2c_wakeup(void);
void DAC_powerdown(void);

#endif

