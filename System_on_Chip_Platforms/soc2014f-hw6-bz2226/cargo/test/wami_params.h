// DO NOT EDIT THIS FILE
#ifndef _WAMI_PARAMS_H_
#define _WAMI_PARAMS_H_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct _rgb_pixel
{
    u16 r, g, b;
} rgb_pixel;

#define RED_CHAN 0
#define GREEN_CHAN 1
#define BLUE_CHAN 2


/*
 * WAMI_DEBAYER_PAD: The number of edge pixels clipped during the
 * debayer process due to not having enough pixels for the full
 * interpolation kernel. Other interpolations could be applied near
 * the edges, but we instead clip the image for simplicity.
 */


#ifndef DEBAYER_SIZE
#define DEBAYER_SIZE 128
#endif

#define WAMI_DEBAYER_IMG_NUM_ROWS DEBAYER_SIZE
#define WAMI_DEBAYER_IMG_NUM_COLS DEBAYER_SIZE

#define WAMI_DEBAYER_PAD 2

#endif