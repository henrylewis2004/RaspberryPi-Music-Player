#ifndef AUDIO_DAC_REGISTERS
#define AUDIO_DAC_REGISTERS

// dac address
#ifndef DAC_ADDR
#define DAC_ADDR 0x18
#endif 

//register values
//pages
#ifndef DAC_REG_PG0
#define DAC_REG_PG0 0x00
#endif 

#ifndef DAC_REG_PG1
#define DAC_REG_PG1 0x01
#endif 

//page 0
#ifndef DAC_VOLUME_CONTROL
#define DAC_VOLUME_CONTROL 0x40
#endif 

#ifndef DAC_LEFT_CHANNEL_REG
#define DAC_LEFT_CHANNEL_REG 0x41
#endif 

#ifndef DAC_RIGHT_CHANNEL_REG
#define DAC_RIGHT_CHANNEL_REG 0x42
#endif 

#ifndef DAC_DATA_PATH_REG
#define DAC_DATA_PATH_REG 0x3F
#endif 

#ifndef DAC_DATA_PATH_VALUE
#define DAC_DATA_PATH_VALUE 0xD4
#endif 

#ifndef DAC_PLL_PROG_PR
#define DAC_PLL_PROG_PR 0x05
#endif 

#ifndef DAC_PLL_PROG_J
#define DAC_PLL_PROG_J 0x06
#endif 

#ifndef DAC_PLL_PROG_D_MSB
#define DAC_PLL_PROG_D_MSB 0x07
#endif 

#ifndef DAC_PLL_PROG_D_LSB
#define DAC_PLL_PROG_D_LSB 0x08
#endif 

#ifndef DAC_CLOCK_MUX1
#define DAC_CLOCK_MUX1 0x04
#endif 

#ifndef DAC_CLOCK_MUX1_SOURCE
#define DAC_CLOCK_MUX1_SOURCE 0b00
#endif 

#ifndef DAC_CODEC_IF
#define DAC_CODEC_IF 0x1B
#endif 

#ifndef DAC_NDAC_REG
#define DAC_NDAC_REG 0x0B
#endif 

#ifndef DAC_MDAC_REG
#define DAC_MDAC_REG 0x0C
#endif 

#ifndef DAC_DOSR_MSB_REG
#define DAC_DOSR_MSB_REG 0x0D
#endif 

#ifndef DAC_DOSR_LSB_REG
#define DAC_DOSR_LSB_REG 0x0E
#endif 

//page 1
#ifndef DAC_HEADPHONES_DRIVER_REG
#define DAC_HEADPHONES_DRIVER_REG 0x1F
#endif 

#ifndef DAC_HEADPHONES_DRIVER_VAL
#define DAC_HEADPHONES_DRIVER_VAL 0xC4
#endif 

#ifndef DAC_OUT_ROUTING_REG
#define DAC_OUT_ROUTING_REG 0x23
#endif // !OUT_ROUTING_REG
       
#ifndef DAC_OUT_ROUTING_VALUE
#define DAC_OUT_ROUTING_VALUE 0x44
#endif // !OUT_ROUTING_REG

#ifndef DAC_HPL_DRIVER
#define DAC_HPL_DRIVER 0x28
#endif 

#ifndef DAC_HPR_DRIVER
#define DAC_HPR_DRIVER 0x29
#endif 

#ifndef DAC_HPL_VOL
#define DAC_HPL_VOL 0x24
#endif 

#ifndef DAC_HPR_VOL
#define DAC_HPR_VOL 0x25
#endif 

#endif
