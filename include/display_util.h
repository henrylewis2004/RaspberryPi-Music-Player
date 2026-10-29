#ifndef DISPLAY_UTIL_H
#define DISPLAY_UTIL_H

void text_size_sanitise(u8g2_t *u8g2, uint max_length, const char *text, char *text_out, size_t out_size);
void time_sanitise(uint seconds_remaining, char *text_out, size_t out_size);

#endif 
