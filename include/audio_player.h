#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

// audio volume settings
// NOTE: needs changing


#ifndef AUDIO_VOLUME_UP
#define AUDIO_VOLUME_UP 5 //db
#endif

#ifndef AUDIO_VOLUME_DOWN
#define AUDIO_VOLUME_DOWN -5 //db
#endif

//methods
void audio_init(void);
void audio_close(void);

void audio_play_noise(void);

bool audio_buffer_refil_requested(void);
void audio_buffer_refil(void);

int audio_play_pause_song(void);
void audio_stop_playback(void);

void audio_volume_up(void);
void audio_volume_down(void);

void audio_skip_song(void);
void audio_add_song_to_queue(char* filepath);
int audio_play_top_queue(void);

bool audio_has_song_selected(void);

#endif // !AUDIO_PLAYER_H
