cmake_minimum_required(VERSION 4.2)

set(PICO_BOARD pico2_w CACHE STRING "Board type")

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(Pico-Music C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

#add_subdirectory(lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src)

add_executable(Pico-Music
	src/main.c
	#	src/led_manager.c
)

target_include_directories(Pico-Music PRIVATE
	#${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/src

)



## link libraries
target_link_libraries(Pico-Music
	pico_stdlib
	pico_cyw43_arch_none
	#	no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
	
)


##
pico_enable_stdio_usb(Pico-Music 1)
pico_enable_stdio_uart(Pico-Music 0)

pico_add_extra_outputs(Pico-Music)




