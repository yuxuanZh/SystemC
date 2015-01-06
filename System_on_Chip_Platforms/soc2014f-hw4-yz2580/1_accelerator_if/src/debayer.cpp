#include "debayer.h"

#define PIXEL_MAX 65535

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

u16 debayer::interp_G_at_RRR_or_G_at_BBB(
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
         2 * bayer_img[row-1][col] +
         2 * bayer_img[row][col-1] +
         4 * bayer_img[row][col] +
         2 * bayer_img[row][col+1] +
         2 * bayer_img[row+1][col];
    const u16 neg =
             bayer_img[row][col+2] +
             bayer_img[row-2][col] +
             bayer_img[row][col-2] +
             bayer_img[row+2][col];

    return compute_and_clamp_pixel(pos, neg);
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
          ((bayer_img[row-2][col] + bayer_img[row+2][col]) >> 1) +
        4 * bayer_img[row][col-1] +
        5 * bayer_img[row][col] +
        4 * bayer_img[row][col+1];
    const u16 neg =
            bayer_img[row-1][col-1] +
            bayer_img[row-1][col+1] +
            bayer_img[row][col-2] +
            bayer_img[row][col+2] +
            bayer_img[row+1][col-1] +
            bayer_img[row+1][col+1];

    return compute_and_clamp_pixel(pos, neg);
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
        4 * bayer_img[row-1][col] +
          ((bayer_img[row][col-2] + bayer_img[row][col+2]) >> 1) +
        5 * bayer_img[row][col] +
        4 * bayer_img[row+1][col];
    const u16 neg =
            bayer_img[row-2][col] +
            bayer_img[row-1][col-1] +
            bayer_img[row-1][col+1] +
            bayer_img[row+1][col-1] +
            bayer_img[row+1][col+1] +
            bayer_img[row+2][col];

    return compute_and_clamp_pixel(pos, neg);
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
        2 * bayer_img[row-1][col-1] +
        2 * bayer_img[row-1][col+1] +
        6 * bayer_img[row][col] +
        2 * bayer_img[row+1][col-1] +
        2 * bayer_img[row+1][col+1];
    const u16 neg =
       (3 * bayer_img[row-2][col] +
        3 * bayer_img[row][col-2] +
        3 * bayer_img[row][col+2] +
        3 * bayer_img[row+2][col]);

    return compute_and_clamp_pixel_fractional_neg(pos, neg);
}

void debayer::wami_debayer(int rows, int cols)
{
	/**
	 * Implement wami_debayer() starting from previous assignments.
	 * HINT: we want CtoS to know the maximum number of iterations
	 * of each loop at compile time; however, since this time the
	 * size of the image can change, make sure your code only runs
	 * the necessary iterations.
	 */
  u32 row,col;
       for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].r = bayer_img[row][col];
        }
    }

    /* Copy top-right green pixels through directly */
    for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].g = bayer_img[row][col];
        }
    }

    /* Copy bottom-left green pixels through directly */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].g = bayer_img[row][col];
        }
    }

    /* Copy blue pixels through directly */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].b = bayer_img[row][col];
        }
    }

    /* Interpolate green pixels at red pixels */
    for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(row, col);
        }
    }

    /* Interpolate green pixels at blue pixels */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].g = interp_G_at_RRR_or_G_at_BBB(row, col);
        }
    }

    /* Interpolate red pixels at green pixels, red row, blue column */
    for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].r = interp_R_at_GRB_or_B_at_GBR(row, col);
        }
    }

    /* Interpolate blue pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].b = interp_R_at_GRB_or_B_at_GBR(row, col);
        }
    }

    /* Interpolate red pixels at green pixels, blue row, red column */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].r = interp_R_at_GBR_or_B_at_GRB(row, col);
        }
    }
 
    /* Interpolate blue pixels at green pixels, red row, blue column */
    for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].b = interp_R_at_GBR_or_B_at_GRB(row, col);
        }
    }

    /* Interpolate red pixels at blue pixels, blue row, blue column */
    for (row = PAD+1; row < rows-PAD; row += 2)
    {
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].r = interp_R_at_BBB_or_B_at_RRR(row, col);
        }
    }

    /* Interpolate blue pixels at red pixels, red row, red column */
    for (row = PAD; row < rows-PAD; row += 2)
    {
        for (col = PAD; col < cols-PAD; col += 2)
        {
            debayer_img[row-PAD][col-PAD].b = interp_R_at_BBB_or_B_at_RRR(row, col);
        }
    }
}


void debayer::config_debayer()
{
	// Initialization at reset
  int pic_size = 0;
  init_done.write(false);
  size.write(0);
  wait();
  cout<< "DUT config_debayer reset @ " << sc_time_stamp() <<endl;
	// Read configuration until done
  do{ wait(); }//this is safer than pic_size = conf_size.read(); here
  while(!conf_done.read());
  pic_size = conf_size.read();
  size.write(pic_size);//write pic_size to signal size
	// Let other threads run then do nothing
	init_done.write(true);
  cout << "DUT config_debayer done @ " << sc_time_stamp() << " pic size " 
       << pic_size << endl;
  
	while (true) {
   wait();
   //when reset jump out of this loop
	}
}


void debayer::load_input()
{
RESET_LOAD:
	// Initialization at reset
  int pic_size = 0;
  int index = 0;
  input_done.write(false);
  bufdin.reset_get();
  rd_index.write(0);
  rd_length.write(0); 
  wait();
  cout<< "DUT load_input reset " << sc_time_stamp() <<endl;
  
	// Wait for init_done from configuration and read image size
  do{ wait(); }
  while(!init_done.read());
  pic_size = size.read();
  rd_length.write(pic_size * pic_size);
  
LOAD_INPUT_WHILE:
	while(true) {
    
    
		// If the entire image has been loaded wait for reset from TB???????????????????????????????????????/
    //????????????????????????????????????????????????????????????????
    
		// Send DMA request
		// Notice that this implementation is still reading the entire image in one burst
    
		// 4-phase handshake load_input() with TB
   
    cout << "DUT send send req to load @ " << endl;
    rd_request.write(true);//request high
    do {wait();}
    while(!rd_grant.read());//wait grant high
    //cout << pic_size << endl;

    rd_request.write(false);//request low
    do {wait();}
    while(rd_grant.read());//wait grant low


    for(int i=0;i<pic_size;i++)//transfer data
    for(int j=0;j<pic_size;j++){
    bayer_img[i][j] = bufdin.get();//blocking
    //cout << "bayer_img " << i* pic_size + (j+1) << " = " << bayer_img[i][j] << endl;
    }
    index = index + rd_length.read();
    //cout << "CHECK it is the next index?????" << index << endl;
    rd_index.write(index);
  
  
    
    cout << "DUT load_input done @ " << sc_time_stamp() << endl;
    
		// Loop over the entire array to get the image pixels
		// Make sure CtoS knows the maximum number of iterations, but
		// do not run useless iterations!

		// 4-phase handshake load_input() with process_debayer()
   input_done.write(true);
   do{wait();}
   while(!process_start.read());
   input_done.write(false);
  
   do {wait();}//this must have. hankshake initialize by self otherwise cannot be stop
   while(rst.read());//wait for DMA reset 
	}
}

void debayer::process_debayer()
{
	// Initialization at reset
  u16 rows = 0;
  int pic_size = 0;
  process_start.write(false);
  process_done.write(false);
  wait();
  cout<< "DUT process_debayer reset " << sc_time_stamp() <<endl;
	// Wait for init_done from configuration and read image size
  do{ wait(); }
  while(!init_done.read());
  pic_size = size.read();

DEBAYER_WHILE:
	while (true) {
 
		// If the entire image has been processed wait for reset from TB
  
		// 4-phase handshake process_debayer() with load_input()
   
    do{wait();}
    while(!input_done.read());
    process_start.write(true);
    do{wait();}
    while(input_done.read());
    process_start.write(false);
    rows = pic_size;
		// Call wami_debayer(). Notice rows is the size of the current image!
		wami_debayer(rows, rows);
		// 4-phase handshake process_debayer() with store_output()
    process_done.write(true);
    do{wait();}
    while(!output_start.read());
    process_done.write(false);
   
    //do {wait();}//this is optional becasuse of handshake req from load but no time penalty as I test
    //while(rst.read());//wait for DMA reset
                            
	}
}

void debayer::store_output()
{
	// Initialization at reset
  int pic_size = 0;
  int index = 0;
  wr_index.write(0);
  wr_length.write(0);
  output_start.write(false);
  debayer_done.write(false);
  bufdout.reset_put();
  wait();
  cout << "DUT store_output reset " << sc_time_stamp() << endl;
	// Wait for init_done from configuration and read image size
  do{ wait(); }
  while(!init_done.read());
  pic_size = size.read();
  wr_length.write((pic_size-2*PAD)*(pic_size-2*PAD));
  
STORE_OUTPUT_WHILE:
	while(true) {
     
		// If the entire image has been sent to TB
		// set debayer_done and wait for reset from TB
   
    //IF THERES NO WAIT MEANS IMMEDIATELY SO. SIZE WILL BE INVALID IF BEFORE HANDSHAKE
    
		// 4-phase handshake store_output() with process_debayer()
    do{wait();}
    while(!process_done.read());
    output_start.write(true);
    do{wait();}
    while(process_done.read());
    output_start.write(false);
    
    //pic_size = size.read();
    
		// Send DMA request

		// 4-phase handshake store_output() with TB
    wr_request.write(true);//request high
    do {wait();}
    while(!wr_grant.read());//wait grant high
    
    wr_request.write(false);//request low
    do {wait();}
    while(wr_grant.read());//wait grant low
    
    for(int i=0;i<pic_size-2*PAD;i++)//transfer data
    for(int j=0;j<pic_size-2*PAD;j++){
    bufdout.put(debayer_img[i][j]);//blocking
    }
    index = index + wr_length.read();
    wr_index.write(index);
    
    

		// Loop over the entire array to send the output image pixels
		// Make sure CtoS knows the maximum number of iterations, but
		// do not run useless iterations!
    debayer_done.write(true);//set debayer_done 
 
    //do {wait();}//optional, the same function as before
    //while(rst.read());//wait for DMA reset
   
    // debayer_done.write(false);//don't have to run to rst directly 
   
	}
}

// Export the module name for CtoS
#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif
