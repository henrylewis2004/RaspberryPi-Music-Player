//c headers
#include <stdio.h>

//pico headers
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"

//project headers
#include "audio_dac_i2s_bridge.h"
#include "dac_i2s.pio.h"
#include "audio_dac_values.h"

// internal \\


static PIO i2s_pio;
static uint i2s_state_machine;

//dma
static uint32_t audio_buffer[2][I2S_BUFFER_WORDS];

#define DMA_CHANNEL_COUNT 2
static int dma_channel[DMA_CHANNEL_COUNT];

static i2s_buffer_callback_t buffer_callback;
static volatile uint buffer_active_half = 0;
//setup

static void dma_irq_handler(void);

static void pio_i2s_init(void){
	i2s_pio = pio1;
	uint offset = pio_add_program(i2s_pio,&i2s_out_program);
	i2s_state_machine = pio_claim_unused_sm(i2s_pio, true);
	i2s_out_program_init(i2s_pio,i2s_state_machine,offset,DAC_SAMPLE_RATE);
}

static void dma_i2s_init(void){
	dma_channel[0] = dma_claim_unused_channel(true);
	dma_channel[1] = dma_claim_unused_channel(true);


	for (int i = 0; i < DMA_CHANNEL_COUNT; i++){
		dma_channel_config dma_cfg = dma_channel_get_default_config(dma_channel[i]);
		channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_32);
		channel_config_set_read_increment(&dma_cfg,true);
		channel_config_set_write_increment(&dma_cfg,false);
		channel_config_set_dreq(&dma_cfg, pio_get_dreq(i2s_pio,i2s_state_machine,true));

		//chain to the other channel (switch between channel 0 and channel 1)
		channel_config_set_chain_to(&dma_cfg,dma_channel[1 - i]);

		dma_channel_configure(
				dma_channel[i],
				&dma_cfg,
				&i2s_pio->txf[i2s_state_machine], //write to PIO TX FIFO
				audio_buffer[i], //read from buffer half i
				I2S_BUFFER_WORDS,
				false
		);

		//enabled irq1 to know when to refil
		dma_channel_set_irq1_enabled(dma_channel[i],true);
	}

	//register IRQ handler
	//irq_set_exclusive_handler(DMA_IRQ_1,dma_irq_handler);
	irq_add_shared_handler(DMA_IRQ_1, dma_irq_handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
	irq_set_enabled(DMA_IRQ_1,true);

	//pre-fill both halves via callback before starting
	if (buffer_callback){
		for (int i = 0; i < DMA_CHANNEL_COUNT; i++){
			buffer_callback(audio_buffer[i],I2S_BUFFER_WORDS);
		}
	}

	// start channel 0 - chains to channel 1
	dma_channel_start(dma_channel[0]);
}

//dma handling
static void dma_irq_handler(void){
	for (int i = 0; i < DMA_CHANNEL_COUNT; i++){
		if (dma_channel_get_irq1_status(dma_channel[i])){
			dma_channel_acknowledge_irq1(dma_channel[i]);

			//refill finished half
			if (buffer_callback){
				buffer_callback(audio_buffer[i],I2S_BUFFER_WORDS);
			}

			//rearm channel
			dma_channel_set_read_addr(dma_channel[i], audio_buffer[i], false);
			dma_channel_set_trans_count(dma_channel[i], I2S_BUFFER_WORDS, false);
		}
	}
}

// public \\

void DAC_i2s_init(i2s_buffer_callback_t callback){
	DAC_set_callback(callback);
	printf("i2s buffer assigned\n");
	pio_i2s_init();
	printf("pio i2s init finished\n");
	dma_i2s_init();
	printf("dma i2s init finished\n");
}

void DAC_set_callback(i2s_buffer_callback_t callback){
	buffer_callback = callback;
}

void DAC_start_dma(void){
	dma_channel_set_irq1_enabled(dma_channel[0], false); //reset dma (just in case? might not be needed)
	dma_channel_set_irq1_enabled(dma_channel[0], true);
	dma_channel_start(dma_channel[0]);
}

/*
void DAC_stop_dma(void){
	dma_channel_set_irq1_enabled(dma_channel[0], false);
	dma_channel_abort(dma_channel[0]);
	dma_channel_cleanup(dma_channel[0]);
}

*/
