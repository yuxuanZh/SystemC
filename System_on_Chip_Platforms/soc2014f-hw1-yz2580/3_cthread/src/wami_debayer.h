#ifndef _WAMI_DEBAYER_H_
#define _WAMI_DEBAYER_H_

#include "wami_params.h"

/* Shorten the name of the Debayer pad for readability */
#define PAD WAMI_DEBAYER_PAD

void wami_debayer(
    rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD],
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS]);

#endif

