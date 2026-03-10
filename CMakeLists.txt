cmake_minimum_required(VERSION 4.2)

set(PICO_BOARD pico2_w CACHE STRING "Board type")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(
	pico_sdk_import.cmake
)

project(Pico-Music C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_subdirectory(
	lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src
)

add_executable(Pico-Music
	src/main.c

	#led
	src/led_manager.c

	# sd - sdio
	src/sd_memory/hw_config.c
	src/sd_memory/sd_memory_manager.c

	#audio - i2s
	src/audio/audio_player.c
	src/audio/audio_dac_i2c_bridge.c
	src/audio/audio_dac_i2s_bridge.c

	#input
	src/input.c
)

target_include_directories(Pico-Music PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/src

)

pico_generate_pio_header(Pico-Music ${CMAKE_CURRENT_LIST_DIR}/src/audio/dac_i2s.pio)



## link libraries
target_link_libraries(Pico-Music
	pico_stdlib
	pico_cyw43_arch_none #for light / wifi module?

	#i2c
	hardware_i2c 

	#sd card & sdio library
	no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 

	#i2s
	hardware_pwm #used to generate mclk for dac

	hardware_pio #used to stream i2s
	hardware_dma
	hardware_irq
	
)



##
pico_enable_stdio_usb(Pico-Music 0)
pico_enable_stdio_uart(Pico-Music 1)

pico_add_extra_outputs(Pico-Music)




