#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#ifndef AUDIO_VOLUME_RAMP_STEPS 
#define AUDIO_VOLUME_RAMP_STEPS 50
#endif

#ifndef AUDIO_VOLUME_STEPS_TIMER_MS 
#define AUDIO_VOLUME_STEPS_TIMER_MS 25
#endif

void audio_play_noise(void);
void audio_init(void);
void audio_close(void);

bool audio_buffer_refil_requested(void);
void audio_buffer_refil(void);

void audio_play_song(char* filepath);
void audio_pause_song(void);
void audio_stop_playback(void);


#endif // !AUDIO_PLAYER_H
