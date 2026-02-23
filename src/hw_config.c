// used to specify the hardware being used
// hardware used = adafruit TLV320DAC3100 I2S DAC

#include "hw_config.h"


//sdio interface
static sd_sdio_if_t sdio_if = {
	//values might need changing
	.CMD_gpio = 18,
	.D0_gpio = 19,
	.baud_rate = 5000000,//125 * 1000 * 1000 / 6 //20833333 Hz
	.SDIO_PIO = pio0,
	.DMA_IRQ_num = DMA_IRQ_0
};

//hardware configuration of sd card (using SDIO interface type / created object above)
static sd_card_t sd_card = {
	.type = SD_IF_SDIO, 
	.sdio_if_p = &sdio_if,

};

//get number of sd cards (project only uses 1 slot, so 1)
size_t sd_get_num(void){
	return 1;
} 


//get sd card by number, in this case only 1 is a valid option
sd_card_t* sd_get_by_num(size_t num){
	if (num == 0){
		return &sd_card;
	}
	else{
		return NULL;
	}


}
