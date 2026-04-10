#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

// audio volume settings
#ifndef AUDIO_VOLUME_UP
#define AUDIO_VOLUME_UP 25 //db
#endif

#ifndef AUDIO_VOLUME_DOWN
#define AUDIO_VOLUME_DOWN -25 //db
#endif

#ifndef AUDIO_VOLUME_RAMP_STEPS  //number of steps to change volume (incrementing by x at a time)
#define AUDIO_VOLUME_RAMP_STEPS 50 
#endif

#ifndef AUDIO_VOLUME_STEPS_TIMER_MS //time to wait between each volume change step
#define AUDIO_VOLUME_STEPS_TIMER_MS 25
#endif


//methods
void audio_init(void);
void audio_close(void);

void audio_play_noise(void);

bool audio_buffer_refil_requested(void);
void audio_buffer_refil(void);

void audio_play_song(char* filepath);
void audio_pause_song(void);
void audio_stop_playback(void);

void audio_volume_up(void);
void audio_volume_down(void);

void audio_skip_song(void);
void audio_add_song_to_queue(char* filepath);


#endif // !AUDIO_PLAYER_H
