#include "debayer.h"

#define PIXEL_MAX 65535

//interp_func
  u16 debayer::interp_G_at_RRR_or_G_at_BBB(      //in order not to use pointer use member variables
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
    //cout << "interp_G_at_RRR_or_G_at_BBB put before @ " << sc_time_stamp()<<endl;
    pos_out.put(pos);
    neg_out.put(neg);
    wait();
    //cout << "interp_G_at_RRR_or_G_at_BBB put after get before @ " << sc_time_stamp()<<endl;
    u16 pixel = pixel_in.get();
    //cout << "interp_G_at_RRR_or_G_at_BBB get after @ " << sc_time_stamp()<<endl;
    return pixel;
}

 u16 debayer::interp_R_at_GRB_or_B_at_GBR(
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
    //cout << "interp_R_at_GRB_or_B_at_GBR put before @ " << sc_time_stamp()<<endl;
    pos_out.put(pos);
    neg_out.put(neg);
    wait();
    //cout << "interp_R_at_GRB_or_B_at_GBR put after get before @ " << sc_time_stamp()<<endl;
    u16 pixel = pixel_in.get();
    //cout << "interp_R_at_GRB_or_B_at_GBR get after @ " << sc_time_stamp()<<endl;
    return pixel;
}
    
 u16 debayer::interp_R_at_GBR_or_B_at_GRB(
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
    //cout << "interp_R_at_GBR_or_B_at_GRB put before @ " << sc_time_stamp()<<endl;
    pos_out.put(pos);
    neg_out.put(neg);
    wait();
    //cout << "interp_R_at_GBR_or_B_at_GRB put after get before @ " << sc_time_stamp()<<endl;
    u16 pixel = pixel_in.get();
    //cout << "interp_R_at_GBR_or_B_at_GRB get after @ " << sc_time_stamp()<<endl;
    return pixel;

}

 u16 debayer::interp_R_at_BBB_or_B_at_RRR(
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
    //cout << "interp_R_at_BBB_or_B_at_RRR put before @ " << sc_time_stamp()<<endl;
    pos_fra_out.put(pos);
    neg_fra_out.put(neg);
    wait();
    //cout << "interp_R_at_BBB_or_B_at_RRR put after get before @ " << sc_time_stamp()<<endl;
    u16 pixel = pixel_fra_in.get();
    //cout << "interp_R_at_BBB_or_B_at_RRR get after @ " << sc_time_stamp()<<endl;
    return pixel;
}


//CTHREAD
 void debayer::compute_and_clamp_pixel()
{
 RESET:
    pos_in.reset_get();
    neg_in.reset_get();
    pixel_out.reset_put();
    cout << "compute_and_clamp_pixel reset @ " <<sc_time_stamp()<<endl;
    wait();
  while(true) 
 {   //wait();
     u16 pos_internal = pos_in.get();
     //wait();//blocking
     u16 neg_internal = neg_in.get();
     u16 pixel;
     if (pos_internal < neg_internal) pixel = 0;
     else
     {
         pixel = (pos_internal - neg_internal) >> 3;
         if (pixel > PIXEL_MAX) pixel = PIXEL_MAX;
     }
     //cout << "compute_and_clamp_pixel put before @ " <<sc_time_stamp()<<endl;
     pixel_out.put(pixel);    
     //cout << "compute_and_clamp_pixel put after @ " <<sc_time_stamp()<<endl;
     wait();
 }
}
 
 
  void debayer::compute_and_clamp_pixel_fractional_neg()
{
 RESET:
     pos_fra_in.reset_get();
     neg_fra_in.reset_get();
     pixel_fra_out.reset_put();
     cout << "compute_and_clamp_pixel_fractional_neg reset @ " <<sc_time_stamp()<<endl;
     wait();
 while(true)
 {  
    //wait();
    u16 pos_internal = pos_fra_in.get();
    u16 neg_internal = neg_fra_in.get();
    u16 pixel;
    
    u32 pos_u32 = ((u32) pos_internal) << 1;
    u32 neg_u32 = (u32) neg_internal;
    if (pos_u32 < neg_u32) pixel = 0;
    else
    {
        pixel = (u16) ((pos_u32 - neg_u32) >> 4);
        if (pixel > PIXEL_MAX) pixel = PIXEL_MAX;
    }
    //cout << "compute_and_clamp_pixel_fractional_neg put before @ " <<sc_time_stamp()<<endl;
    pixel_fra_out.put(pixel);
    //cout << "compute_and_clamp_pixel_fractional_neg put after @ " <<sc_time_stamp()<<endl;
    wait();
}
}

 void debayer::wami_debayer()
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
            debayered[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(row, col);
        }
    }

    /* Interpolate green pixels at blue pixels */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(row, col);
        }
    }

    /* Interpolate red pixels at green pixels, red row, blue column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_GRB_or_B_at_GBR(row, col);
        }
    }

    /* Interpolate blue pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_GRB_or_B_at_GBR(row, col);
        }
    }

    /* Interpolate red pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_GBR_or_B_at_GRB(row, col);     
        }
    }
 
    /* Interpolate blue pixels at green pixels, red row, blue column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_GBR_or_B_at_GRB(row, col);
        }
    }

    /* Interpolate red pixels at blue pixels, blue row, blue column */
    for (row = PAD+1; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].r = interp_R_at_BBB_or_B_at_RRR(row, col);
        }
    }

    /* Interpolate blue pixels at red pixels, red row, red column */
    for (row = PAD; row < WAMI_DEBAYER_IMG_NUM_ROWS-PAD; row += 2)
    {
        for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
        {
            debayered[row-PAD][col-PAD].b = interp_R_at_BBB_or_B_at_RRR(row, col);
        }
    }
}

void debayer::process_debayer()
{
 RESET:

   data_in.reset_get();
   data_out.reset_put();
   
   pos_out.reset_put();
   neg_out.reset_put();
   pixel_in.reset_get();
   
   pos_fra_out.reset_put();
   neg_fra_out.reset_put();
   pixel_fra_in.reset_get();
   
   for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
   for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++)
    bayer[i][j] = 0;
   for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD; i++)
   for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD; j++){
    debayered[i][j].r = 0;
    debayered[i][j].g = 0;
    debayered[i][j].b = 0;
   }
   cout<< "DUT reset " << sc_time_stamp() <<endl;
   wait();
   
   while(true) {
		//u16 bayer[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
		//rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];

   INPUT:
    //u16 tmpInput = data_in.get();
    cout<< "DUT get before @ " << sc_time_stamp() << endl;
   	for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++){
			bayer[i][j] = data_in.get();
      wait();
      }
    cout<< "DUT get after @ " << sc_time_stamp() <<endl;
    
    // Embed here functions from wami_debayer.c
    wami_debayer();//pass no pointer
 
    cout<< "DUT put before @ " << sc_time_stamp() <<endl;
    
    OUTPUT:
    for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD; j++){
     data_out.put(debayered[i][j]);
     wait();
     }
    cout<< "DUT put after @ " << sc_time_stamp() <<endl;
    }
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif

