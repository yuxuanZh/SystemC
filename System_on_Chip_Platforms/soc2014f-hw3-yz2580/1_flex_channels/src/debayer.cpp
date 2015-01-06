#include "debayer.h"

#define PIXEL_MAX 65535

//wami_debayer functions
static u16 compute_and_clamp_pixel(
    u16 pos,
    u16 neg)
{
    if (pos < neg)
    {
        return 0;
    }
    else
    {
        const u16 pixel = (pos - neg) >> 3;
        if (pixel > PIXEL_MAX) { return PIXEL_MAX; }
        else { return pixel; }
    }
}

/*
 * This version handles masks with fractional negative values. In those
 * cases truncating before subtraction does not generally yield the
 * same result as truncating after subtraction.  The negative value
 * is using weights in units of 1/16ths so that the one-half portions
 * are retained.
 */
static u16 compute_and_clamp_pixel_fractional_neg(
    u16 pos,
    u16 neg)
{
    /*
     * The positive portion is converted to u32 prior to doubling because
     * otherwise some of the weights could yield overflow. At that point,
     * all weights are effectively 16x their actual value, so combining
     * the positive and negative portions and then shifting by four bits
     * yields the equivalent of a floor() applied to the result of the
     * full precision convolution.
     */
    const u32 pos_u32 = ((u32) pos) << 1;
    const u32 neg_u32 = (u32) neg;
    if (pos_u32 < neg_u32)
    {
        return 0;
    }
    else
    {
        const u16 pixel = (u16) ((pos_u32 - neg_u32) >> 4);
        if (pixel > PIXEL_MAX) { return PIXEL_MAX; }
        else { return pixel; }
    }
}

static u16 interp_G_at_RRR_or_G_at_BBB(
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS],
    u32 row,
    u32 col)
{
    /*
     * The mask to interpolate G at R or B is:
     *
     * [0  0 -1  0  0
     *  0  0  2  0  0
     * -1  2  4  2 -1
     *  0  0  2  0  0
     *  0  0 -1  0  0]/8
     */
    const u16 pos =
         2 * bayer[row-1][col] +
         2 * bayer[row][col-1] +
         4 * bayer[row][col] +
         2 * bayer[row][col+1] +
         2 * bayer[row+1][col];
    const u16 neg =
             bayer[row][col+2] +
             bayer[row-2][col] +
             bayer[row][col-2] +
             bayer[row+2][col];

    return compute_and_clamp_pixel(pos, neg);
}

static u16 interp_R_at_GRB_or_B_at_GBR(
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS],
    u32 row,
    u32 col)
{
    /*
     * [0  0 0.5 0  0
     *  0 -1  0 -1  0
     * -1  4  5  4 -1
     *  0 -1  0 -1  0
     *  0  0 0.5 0  0]/8;
     */
    const u16 pos =
          ((bayer[row-2][col] + bayer[row+2][col]) >> 1) +
        4 * bayer[row][col-1] +
        5 * bayer[row][col] +
        4 * bayer[row][col+1];
    const u16 neg =
            bayer[row-1][col-1] +
            bayer[row-1][col+1] +
            bayer[row][col-2] +
            bayer[row][col+2] +
            bayer[row+1][col-1] +
            bayer[row+1][col+1];

    return compute_and_clamp_pixel(pos, neg);
}
    
static u16 interp_R_at_GBR_or_B_at_GRB(
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS],
    u32 row,
    u32 col)
{
    /*
     * [0  0 -1  0  0
     *  0 -1  4 -1  0
     * 0.5 0  5  0 0.5
     *  0 -1  4 -1  0
     *  0  0 -1 0  0]/8;
     */
    const u16 pos =
        4 * bayer[row-1][col] +
          ((bayer[row][col-2] + bayer[row][col+2]) >> 1) +
        5 * bayer[row][col] +
        4 * bayer[row+1][col];
    const u16 neg =
            bayer[row-2][col] +
            bayer[row-1][col-1] +
            bayer[row-1][col+1] +
            bayer[row+1][col-1] +
            bayer[row+1][col+1] +
            bayer[row+2][col];

    return compute_and_clamp_pixel(pos, neg);
}

static u16 interp_R_at_BBB_or_B_at_RRR(
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS],
    u32 row,
    u32 col)
{
    /*
     * [0  0 -1.5 0  0
     *  0  2  0  2  0
     * -1.5 0  6  0 -1.5
     *  0  2  0  2  0
     *  0  0 -1.5 0  0]/8;
     */
    const u16 pos =
        2 * bayer[row-1][col-1] +
        2 * bayer[row-1][col+1] +
        6 * bayer[row][col] +
        2 * bayer[row+1][col-1] +
        2 * bayer[row+1][col+1];
    const u16 neg =
       (3 * bayer[row-2][col] +
        3 * bayer[row][col-2] +
        3 * bayer[row][col+2] +
        3 * bayer[row+2][col]);

    return compute_and_clamp_pixel_fractional_neg(pos, neg);
}

void wami_debayer(
    rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD],
    u16 (* const bayer)[WAMI_DEBAYER_IMG_NUM_COLS])
{
    u32 row, col;

    /*
     * Demosaic the following Bayer pattern:
     * R G ...
     * G B ...
     * ... ...
     */

    /* Copy red pixels through directly */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = bayer[row][col];
        }
    }

    /* Copy top-right green pixels through directly */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].g = bayer[row][col];
        }
    }

    /* Copy bottom-left green pixels through directly */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].g = bayer[row][col];
        }
    }

    /* Copy blue pixels through directly */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = bayer[row][col];
        }
    }

    /* Interpolate green pixels at red pixels */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(
                bayer, row, col);
        }
    }

    /* Interpolate green pixels at blue pixels */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(
                bayer, row, col);
        }
    }

    /* Interpolate red pixels at green pixels, red row, blue column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_GRB_or_B_at_GBR(
                bayer, row, col);
        }
    }

    /* Interpolate blue pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_GRB_or_B_at_GBR(
                bayer, row, col);
        }
    }

    /* Interpolate red pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_GBR_or_B_at_GRB(
                bayer, row, col);
        }
    }
 
    /* Interpolate blue pixels at green pixels, red row, blue column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_GBR_or_B_at_GRB(
                bayer, row, col);
        }
    }

    /* Interpolate red pixels at blue pixels, blue row, blue column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_BBB_or_B_at_RRR(
                bayer, row, col);
        }
    }

    /* Interpolate blue pixels at red pixels, red row, red column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_BBB_or_B_at_RRR(
                bayer, row, col);
        }
    }
}


void debayer::process_debayer()
{
 RESET:

   data_in.reset_get();
   data_out.reset_put();
   cout<< "DUT reset " << sc_time_stamp() <<endl;
   wait();
   
   while(true) {
		u16 bayer[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
		rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];
    //u32 row, col;
    

   INPUT:
    //u16 tmpInput = data_in.get();
    cout<< "DUT get before " << sc_time_stamp() <<endl;
   	for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++){
			bayer[i][j] = data_in.get();
      wait();
      }
    cout<< "DUT get after " << sc_time_stamp() <<endl;
   // Embed here functions from wami_debayer.c
    wami_debayer(debayered,bayer);
   
    cout<< "DUT put before " << sc_time_stamp() <<endl;
    OUTPUT:
    for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD; j++){
     data_out.put(debayered[i][j]);
     wait();
     }
    cout<< "DUT put after " << sc_time_stamp() <<endl;
    }
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif

