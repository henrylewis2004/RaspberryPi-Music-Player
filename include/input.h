#ifndef INPUT_H
#define INPUT_H

#ifndef PLAY_BUTTON_PIN
#define PLAY_BUTTON_PIN 15
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
       

void input_init(void);

#endif
