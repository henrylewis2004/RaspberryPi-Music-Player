#ifndef AUDIO_DAC_PINS_H
#define AUDIO_DAC_PINS_H
#endif

// register address
#ifndef DAC_ADDR
#define DAC_ADDR 0x18
#endif 

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
#define DAC_DIN_GPIO_PIN 8
#endif

#ifndef DAC_WSEL_GPIO_PIN
#define DAC_WSEL_GPIO_PIN 7
#endif

#ifndef DAC_BCK_GPIO_PIN
#define DAC_BCK_GPIO_PIN 6
#endif

/* emulate with PLL
#ifndef DAC_MCLK_GPIO_PIN
#define DAC_MCLK_GPIO_PIN 11
#endif
*/


//i2s audio values
#ifndef DAC_SAMPLE_RATE
#define DAC_SAMPLE_RATE 44100000
#endif 

#ifndef DAC_BIT_DEPTH
#define DAC_BIT_DEPTH 16
#endif 

//register values
#ifndef DAC_REG_PG0
#define DAC_REG_PG0 0x00
#endif 

#ifndef DAC_VOLUME_CONTROL
#define DAC_VOLUME_CONTROL 0x40
#endif 

#ifndef DAC_LEFT_CHANNEL_REG
#define DAC_LEFT_CHANNEL_REG 0x41
#endif 

#ifndef DAC_RIGHT_CHANNEL_REG
#define DAC_RIGHT_CHANNEL_REG 0x41
#endif 

#ifndef DAC_DATA_PATH_REG
#define DAC_DATA_PATH_REG 0x3F
#endif 

#ifndef DAC_DATA_PATH_VALUE
#define DAC_DATA_PATH_VALUE 0xD4
#endif 
