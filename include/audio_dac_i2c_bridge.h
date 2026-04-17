#ifndef AUDIO_DAC_I2C_BRIDGE_H
#define AUDIO_DAC_I2C_BRIDGE_H

#ifndef DEFAULT_AUDIO_VOLUME
#define DEFAULT_AUDIO_VOLUME -35 //db
#endif

#ifndef VOLUME_RAMP_QUICK_STEP_COUNT
#define VOLUME_RAMP_QUICK_STEP_COUNT 10
#endif

#ifndef VOLUME_RAMP_QUICK_STEP_TIME_MS
#define VOLUME_RAMP_QUICK_STEP_TIME_MS 5
#endif

#ifndef VOLUME_RAMP_STEP_COUNT
#define VOLUME_RAMP_STEP_COUNT 20
#endif

#ifndef VOLUME_RAMP_STEP_TIME_MS
#define VOLUME_RAMP_STEP_TIME_MS 10
#endif

int dac_mute(bool mute);
int set_channel_volume(bool right_channel, float volume_db);
float get_dac_volume(void);

int ramp_set_dac_volume(float target_volume_db,uint steps, uint step_timer_ms);
int dac_set_volume_quick(float target_volume_db);
int dac_set_volume(float target_volume_db);

void DAC_i2c_wakeup(void);
void DAC_powerdown(void);

#endif

