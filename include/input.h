#ifndef INPUT_H
#define INPUT_H

#ifndef PLAY_BUTTON_PIN
#define PLAY_BUTTON_PIN 26
#endif // !PLAY_BUTTON_PIN
       
#ifndef SKIP_SONG_BUTTON_PIN
#define SKIP_SONG_BUTTON_PIN X
#endif // !SKIP_SONG_BUTTON_PIN
       
#ifndef V_BUTTON_UP_PIN
#define V_BUTTON_UP_PIN X
#endif // !VOLUME_UP_BUTTON_PIN
      
#ifndef V_BUTTON_DOWN_PIN
#define V_BUTTON_DOWN_PIN X
#endif // !VOLUME_DOWN_BUTTON_PIN
       
#ifndef HOME_BUTTON_PIN
#define HOME_BUTTON_PIN X
#endif // !HOME_BUTTON_PIN
       
#ifndef ANALOG_STICK_PIN
#define ANALOG_STICK_PIN X
#endif // !2D_ANALOG_STICK_PIN
       
//structs
typedef struct {
	bool play_button;
	bool skip_button;

	bool vol_up_button;
	bool vol_down_button;

	bool power_button;
} input_flags;

//functions
void input_init(void);
void input_reset_flags(void);
input_flags input_get_flags(void);
input_flags input_get_reset_flags(void);

#endif
