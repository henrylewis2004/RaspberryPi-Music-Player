// c headers
#include <stdio.h>
#include <string.h>

// library headers
#include "pico/stdlib.h"
#include "u8g2.h"

// project headers
#include "display_util.h"

void text_size_sanitise(u8g2_t *u8g2, uint max_length, const char *text, char *text_out, size_t out_size){
	if (out_size <= 0){
		return;
	}

	if (text == NULL){
		text_out[0] = '\0';
		return;
	}

	snprintf(text_out, out_size, "%s", text);

	if (u8g2_GetStrWidth(u8g2, text_out) <= max_length){
		return;
	}

	size_t len = strlen(text_out);

	while (len > 0) {
		text_out[--len] = '\0';

		char c[256];
		snprintf(c, sizeof(c), "%s...", text_out);

		if (u8g2_GetStrWidth(u8g2, c) <= max_length) {
		    snprintf(text_out, out_size, "%s", c);
		    return;
		}
	}

	text_out[0] = '\0';

}

//void time_sanitise(){}
