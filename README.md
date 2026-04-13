# RaspberryPi-Music-Player
A music player using a RaspberryPi Pico 2 W

## Libraries used

[no-OS-FatFS-SD-SDIO-SPI-RPI-Pico by carlk3](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/tree/main) - used for SDIO Micro SD card interface


[Adafruit CircuitPython TLV320](https://github.com/adafruit/Adafruit_CircuitPython_TLV320/tree/main) - referenced and then used to configure the audio DAC in C

## Progress

For progress see [todo.md](https://github.com/henrylewis2004/RaspberryPi-Music-Player/blob/main/todo.md)

## Components Used
| Function | Component | Aquired |
|-|-|-|
| Microcontroller  | [Raspberry Pi Pico 2 W](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)| X |
| Audio DAC | [Adafruit TLV320DAC3100](https://www.adafruit.com/product/6309?srsltid=AfmBOorzJw4PwT3HF0vGFX7eqOSgBaOBtBcg8bkU7igFjqLHTnfaYH3e)| X |
| Memory Storage | [Adafruit Micro SD SPI or SDIO Card Breakout Board](https://www.adafruit.com/product/4682?srsltid=AfmBOoperZd8N_q_g0DlBwCeuqUr0O1IKYDF3KCDKo4BibNNp2DtdHoi)| X |
| Micro SD Card | Some Micro SD Card (Formatted to FAT32) | X |
| Screen | *TBD* | O |
| Battery | [2000mAh 3.7V LiPo Battery](https://thepihut.com/products/2000mah-3-7v-lipo-battery) | O |
| Battery Charger | *TBD* | O |
| Input Buttons | [2-Axis Joystick](https://thepihut.com/products/psp-2-axis-analog-thumb-joystick), [An on/off switch](https://thepihut.com/products/breadboard-friendly-spdt-slide-switch), [3x Buttons](https://thepihut.com/products/180-piece-ultimate-tactile-button-kit), *POTENTIALLY MORE TBD* | O |

## Wiring

### Microcontroller - [Raspberry Pi Pico 2 W](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)

<p align="left">
    <img src="https://github.com/henrylewis2004/RaspberryPi-Music-Player/blob/main/.github/images/pico2w-pinout.svg" width=600 />
</p>

| PICO PIN | Board PIN | PICO PIN | Board PIN |
|-|-|-|-|
| 1 (GP0) | x | 40 (VBUS) | x |
| 2 (GP1) | x | 39 (VSYS) | x |
| 3 (GND) | x | 38 (GND) | Common Ground |
| 4 (GP2) | x | 37 (3V3_EN) | x |
| 5 (GP3) | DAC RST | 36 (3V3(OUT)) | Common 3V |
| 6 (GP4) | DAC SDA | 35 () | x |
| 7 (GP5) | DAC SCL | 34 (GP28) | x |
| 8 (GND) | x | 33 (GND) | x |
| 9 (GP6) | DAC DIN | 32 (GP27) | x |
| 10 (GP7) | DAC BCK | 31 (GP26) | x |
| 11 (GP8) | DAC WSEL | 30 (RUN) | x |
| 12 (GP9) | x | 29 (GP22) | SD D3 |
| 13 (GND) | x | 28 (GND) | x |
| 14 (GP10) | DAC MCLK | 27 (GP21) | SD D2 |
| 15 (GP11) | x | 26 (GP20) | SD D1 |
| 16 (GP12) | x | 25 (GP19) | SD D0 |
| 17 (GP13) | x | 24 (GP18) | SD CMD |
| 18 (GND) | x | 23 (GND) | x |
| 19 (GP14) | x | 22 (GP17) | SD CLK |
| 20 (GP15) | x | 21 (GP16) | SD DET |

> NOTE:
> Common ground pin (38) might need changing when adding more components.

### Memory Storage - [Adafruit Micro SD SPI or SDIO Card Breakout Board](https://www.adafruit.com/product/4682?srsltid=AfmBOoperZd8N_q_g0DlBwCeuqUr0O1IKYDF3KCDKo4BibNNp2DtdHoi)

<p align="left">
    <img src="https://github.com/henrylewis2004/RaspberryPi-Music-Player/blob/main/.github/images/adafruit_microsd_pins.jpg" width=600 />
</p>

| Board PIN | Pico PIN | GPIO PIN |
|-|-|-|
| 3V | 3V3(OUT) - 36 | n/a |
| GND | GND - 38 | n/a |
| DET | 21 | 16 |
| CLK | 22 | 17 | (using 47ohm resistor)
| CMD | 24 | 18 |
| D0 | 25 | 19 |
| D1 | 26 | 20 |
| D2 | 27 | 21 |
| D3 | 29 | 22 |

### Audio Dac - [Adafruit TLV320DAC3100](https://www.adafruit.com/product/6309?srsltid=AfmBOorzJw4PwT3HF0vGFX7eqOSgBaOBtBcg8bkU7igFjqLHTnfaYH3e)

<p align="left">
    <img src="https://github.com/henrylewis2004/RaspberryPi-Music-Player/blob/main/.github/images/adafruit_dac_pins.jpg" width=600 />
</p>

| Board PIN | Pico PIN | GPIO PIN |
|-|-|-|
| VIN  | 3V3(OUT) - 36 | n/a |
| GND | GND - 38 | n/a |
| RST | 5 | 3 |
| SDA | 6 | 4 |
| SCL | 7 | 5 |
| DIN | 9 | 6 |
| WSEL | 11 | 8 |
| BCK | 10 | 7 |
| MCLK | 14 | 10 |
