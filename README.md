# RaspberryPi-Music-Player
A music player using a RaspberryPi Pico 2 W



## Components Used
| Function | Component | Aquired |
|-|-|-|
| Microcontroller  | [Raspberry Pi Pico 2 W](https://www.raspberrypi.com/products/raspberry-pi-pico-2/)| X |
| Audio DAC | [adafruit TLV320DAC3100](https://www.adafruit.com/product/6309?srsltid=AfmBOorzJw4PwT3HF0vGFX7eqOSgBaOBtBcg8bkU7igFjqLHTnfaYH3e)| X |
| Memory Storage | [Adafruit Micro SD SPI or SDIO Card Breakout Board](https://www.adafruit.com/product/4682?srsltid=AfmBOoperZd8N_q_g0DlBwCeuqUr0O1IKYDF3KCDKo4BibNNp2DtdHoi)| X |
| Micro SD Card | Some Micro SD Card (Formatted to FAT32) | X |
| Screen | *TBD* | O |
| Battery | [2000mAh 3.7V LiPo Battery](https://thepihut.com/products/2000mah-3-7v-lipo-battery) | O |
| Battery Charger | *TBD* | O |
| Input Buttons | [2-Axis Joystick](https://thepihut.com/products/psp-2-axis-analog-thumb-joystick), [An on/off switch](https://thepihut.com/products/breadboard-friendly-spdt-slide-switch), [3x Buttons](https://thepihut.com/products/180-piece-ultimate-tactile-button-kit), *POTENTIALLY MORE TBD* | O |

## Wiring

### Memory Storage - [Adafruit Micro SD SPI or SDIO Card Breakout Board](https://www.adafruit.com/product/4682?srsltid=AfmBOoperZd8N_q_g0DlBwCeuqUr0O1IKYDF3KCDKo4BibNNp2DtdHoi)

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
