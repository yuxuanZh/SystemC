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

#define SMALL 1
#define MEDIUM 2
#define LARGE 3

#ifndef INPUT_SIZE
#define INPUT_SIZE SMALL
#endif

/*
 * WAMI_DEBAYER_PAD: The number of edge pixels clipped during the
 * debayer process due to not having enough pixels for the full
 * interpolation kernel. Other interpolations could be applied near
 * the edges, but we instead clip the image for simplicity.
 */

#if INPUT_SIZE == SMALL
#define WAMI_DEBAYER_IMG_NUM_ROWS 512
#define WAMI_DEBAYER_IMG_NUM_COLS 512
#define WAMI_GMM_IMG_NUM_ROWS 512
#define WAMI_GMM_IMG_NUM_COLS 512

#elif INPUT_SIZE == MEDIUM
#define WAMI_DEBAYER_IMG_NUM_ROWS 1024
#define WAMI_DEBAYER_IMG_NUM_COLS 1024
#define WAMI_GMM_IMG_NUM_ROWS 1024
#define WAMI_GMM_IMG_NUM_COLS 1024

#elif INPUT_SIZE == LARGE
#define WAMI_DEBAYER_IMG_NUM_ROWS 2048
#define WAMI_DEBAYER_IMG_NUM_COLS 2048
#define WAMI_GMM_IMG_NUM_ROWS 2048
#define WAMI_GMM_IMG_NUM_COLS 2048

#else
#error "Unhandled value for INPUT_SIZE"
#endif

#define WAMI_GMM_NUM_MODELS 5

#define WAMI_GMM_NUM_FRAMES 5

#define WAMI_DEBAYER_PAD 2

#endif
