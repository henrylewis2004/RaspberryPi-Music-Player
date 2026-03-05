#ifndef AUDIO_DAC_PINS_H
#define AUDIO_DAC_PINS_H

//i2c port 
#ifndef I2C_PORT
#define I2C_PORT i2c0
#endif 

// pins

//reset pin
#ifndef DAC_RESET_GPIO_PIN
#define DAC_RESET_GPIO_PIN 3
#endif

//i2c pins
#ifndef DAC_SCL_GPIO_PIN
#define DAC_SCL_GPIO_PIN 5
#endif

#ifndef DAC_SDA_GPIO_PIN
#define DAC_SDA_GPIO_PIN 4
#endif

//i2s pins
#ifndef DAC_DIN_GPIO_PIN
#define DAC_DIN_GPIO_PIN 6
#endif

#ifndef DAC_WSEL_GPIO_PIN
#define DAC_WSEL_GPIO_PIN 8
#endif

#ifndef DAC_BCK_GPIO_PIN
#define DAC_BCK_GPIO_PIN 7
#endif

#ifndef DAC_MCLK_GPIO_PIN
#define DAC_MCLK_GPIO_PIN 10
#endif

//i2s audio values
#ifndef DAC_SAMPLE_RATE
#define DAC_SAMPLE_RATE 44100
#endif 

#endif
