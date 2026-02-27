# RaspberryPi-Music-Player
A music player using a RaspberryPi Pico 2 W

## Libraries used

[no-OS-FatFS-SD-SDIO-SPI-RPI-Pico by carlk3](https://github.com/carlk3/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/tree/main) - used for SDIO Micro SD card interface
[Adafruit CircuitPython TLV320](https://github.com/adafruit/Adafruit_CircuitPython_TLV320/tree/main) - adapted into C and then used to configure the audio DAC


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

### Memory Storage - [Adafruit Micro SD SPI or SDIO Card Breakout Board](https://www.adafruit.com/product/4682?srsltid=AfmBOoperZd8N_q_g0DlBwCeuqUr0O1IKYDF3KCDKo4BibNNp2DtdHoi)

<p align="left">
    <img src="https://github.com/henrylewis2004/RaspberryPi-Music-Player/blob/main/.github/images/adafruit_microsd_pins.jpg" width=600 />
</p>

| Board PIN | Pico PIN | GPIO PIN |
|-|-|-|
| 3V | 3V3(OUT) - 36 | n/a |
| GND | GND - 38 | n/a |
| DET | 21 | 16 |
| CLK | 22 | 17 |
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
| DIN | 11 | 8 |
| WSEL | 12 | 9 |
| BCK | 14 | 10 |
| MCLK | 15 | 11 |
