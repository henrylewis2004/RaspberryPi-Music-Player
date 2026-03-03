#ifndef AUDIO_DAC_BRIDGE_H
#define AUDIO_DAC_BRIDGE_H

int dac_mute(bool mute);
int set_channel_volume(bool right_channel, float volume_db);

void DAC_i2c_wakeup(void);
void DAC_powerdown(void);

#endif

