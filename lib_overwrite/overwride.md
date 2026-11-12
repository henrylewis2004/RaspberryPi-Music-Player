# Overwride

Some library files need to overwritten to work with the project.

## u8g2

To fit onto the pico's flash memory, the number of loaded fonts need to be reduced. This is done by modifying the CMakeLists.txt and u8g2_font_ncenB08_tr.c.
