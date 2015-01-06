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



/**
 * Notice that every large array (local memory) access
 * has to be followed by an explicit wait(). Real memories
 * are usually synchronous and require at least one clock
 * cycle in order to access or write a location.
 * By default CtoS scheduler is not allowed to move memory
 * accesses, thus scheduling fails if trying to access memory
 * without an explicit wait().
 * Multiple read / write could occur during the same cycle
 * if the memory has more than one port.
 * For this assignment we only use standard memories with
 * two ports; ports are bound automatically to processes by
 * CtoS, thus one port of the circular buffer is used by
 * process load_input(), while the other is used by
 * process_debayer().
 */
u16 debayer::interp_G_at_RRR_or_G_at_BBB(u32 row, u32 col)
{
/*
     * [0  0 -1  0  0
     *  0 -1  4 -1  0
     * 0.5 0  5  0 0.5
     *  0 -1  4 -1  0
     *  0  0 -1 0  0]/8;
     */
     
     //Consider cases of overflow and underflow
     
     
     //cout << "row " << row << " col " << col << endl;
     u32 row_add_1, row_add_2, row_min_1, row_min_2;
     u16 pos, neg;
     
     if (row == 0) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1 + 6;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 1) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 4) {
        row_add_2 = row + 2 - 6;
        row_add_1 = row + 1;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
        }
     else if(row == 5) {
        row_add_1 = row + 1 - 6;
        row_add_2 = row + 2 - 6;
        row_min_1 = row - 1;
        row_min_2 = row - 2;  
        }   
     else   {
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
         }
     // cout << row_add_1 << " " <<  row_add_2 << " " << row_min_1 << " " <<  row_min_2 << endl;
    /*const u16 pos =
         2 * bayer_img[row_min_1][col] +
         2 * bayer_img[row][col-1] +
         4 * bayer_img[row][col] +
         2 * bayer_img[row][col+1] +
         2 * bayer_img[row_add_1][col];
     */    
     
       pos = 2 * bayer_img[row_min_1][col];
       wait();//schedule for syn
       pos = pos +  2 * bayer_img[row][col-1];
       wait();
       pos = pos +  4 * bayer_img[row][col];
       wait();
       pos = pos +  2 * bayer_img[row][col+1];
       wait();
       pos = pos +  2 * bayer_img[row_add_1][col];
     
     
         wait();//scheduling memory
    
      neg = bayer_img[row][col+2];
      wait();
      neg = neg + bayer_img[row_min_2][col];
      wait();
      neg = neg+ bayer_img[row][col-2];
      wait();
      neg = neg + bayer_img[row_add_2][col];

    return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_GRB_or_B_at_GBR(u32 row, u32 col)
{
/*
     * [0  0 0.5 0  0
     *  0 -1  0 -1  0
     * -1  4  5  4 -1
     *  0 -1  0 -1  0
     *  0  0 0.5 0  0]/8;
     */
     
     u32 row_add_1, row_add_2, row_min_1, row_min_2;
     u16 pos, neg;
     
    if (row == 0) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1 + 6;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 1) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 4) {
        row_add_2 = row + 2 - 6;
        row_add_1 = row + 1;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
        }
     else if(row == 5) {
        row_add_1 = row + 1 - 6;
        row_add_2 = row + 2 - 6;
        row_min_1 = row - 1;
        row_min_2 = row - 2;  
        }   
     else   {
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
         }
    
    /*     
    const u16 pos =
          ((bayer_img[row_min_2][col] + bayer_img[row_add_2][col]) >> 1) +
        4 * bayer_img[row][col-1] +
        5 * bayer_img[row][col] +
        4 * bayer_img[row][col+1];
    */
    pos = bayer_img[row_min_2][col];
    wait();
    pos = (pos + bayer_img[row_add_2][col]) >> 1;
    wait();
    pos = pos + 4 * bayer_img[row][col-1];
    wait();
    pos = pos + 5 * bayer_img[row][col];
    wait();
    pos = pos + 4 * bayer_img[row][col+1];
            
        wait();//scheduling memory
        
    neg = bayer_img[row_min_1][col-1];
    wait();
    neg = neg + bayer_img[row_min_1][col+1];
    wait();
    neg = neg + bayer_img[row][col-2];
    wait();
    neg = neg + bayer_img[row][col+2];
    wait();
    neg = neg + bayer_img[row_add_1][col-1];
    wait();
    neg = neg + bayer_img[row_add_1][col+1];

    return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_GBR_or_B_at_GRB(u32 row,  u32 col)
{
 /*
     * [0  0 -1  0  0
     *  0 -1  4 -1  0
     * 0.5 0  5  0 0.5
     *  0 -1  4 -1  0
     *  0  0 -1 0  0]/8;
     */
     
 
      u32 row_add_1, row_add_2, row_min_1, row_min_2;
      u16 pos, neg;
    if (row == 0) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1 + 6;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 1) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 4) {
        row_add_2 = row + 2 - 6;
        row_add_1 = row + 1;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
        }
     else if(row == 5) {
        row_add_1 = row + 1 - 6;
        row_add_2 = row + 2 - 6;
        row_min_1 = row - 1;
        row_min_2 = row - 2;  
        }   
     else   {
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
         }
 /*
    const u16 pos =
        4 * bayer_img[row_min_1][col] +
          ((bayer_img[row][col-2] + bayer_img[row][col+2]) >> 1) +
        5 * bayer_img[row][col] +
        4 * bayer_img[row_add_1][col];
  */      
  
   pos = bayer_img[row][col-2]; 
   wait();
   pos = (pos + bayer_img[row][col+2])  >> 1;
   wait();
   pos = pos + 4 * bayer_img[row_min_1][col];
   wait();
   pos = pos + 5 * bayer_img[row][col];
   wait();    
   pos = pos + 4 * bayer_img[row_add_1][col];

        
        wait();//scheduling memory
        
  neg = bayer_img[row_min_2][col];
  wait();
  neg = neg + bayer_img[row_min_1][col-1];
  wait();
  neg = neg + bayer_img[row_min_1][col+1];
  wait();
  neg = neg + bayer_img[row_add_1][col-1];
  wait();
  neg = neg + bayer_img[row_add_1][col+1];
  wait();
  neg = neg + bayer_img[row_add_2][col];
  /*      
    const u16 neg =
            bayer_img[row_min_2][col] +
            bayer_img[row_min_1][col-1] +
            bayer_img[row_min_1][col+1] +
            bayer_img[row_add_1][col-1] +
            bayer_img[row_add_1][col+1] +
            bayer_img[row_add_2][col];
  */
    return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_BBB_or_B_at_RRR(u32 row, u32 col)
{
 /*
     * [0  0 -1.5 0  0
     *  0  2  0  2  0
     * -1.5 0  6  0 -1.5
     *  0  2  0  2  0
     *  0  0 -1.5 0  0]/8;
     */
     //cout << "CHECK 8" << "row " << row << "col " << col << endl;
     
     u32 row_add_1, row_add_2, row_min_1, row_min_2;
     u16 pos, neg;
     
    if (row == 0) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1 + 6;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 1) {
        row_min_2 = row - 2 + 6;
        row_min_1 = row - 1;
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        }
     else if(row == 4) {
        row_add_2 = row + 2 - 6;
        row_add_1 = row + 1;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
        }
     else if(row == 5) {
        row_add_1 = row + 1 - 6;
        row_add_2 = row + 2 - 6;
        row_min_1 = row - 1;
        row_min_2 = row - 2;  
        }   
     else   {
        row_add_1 = row + 1;
        row_add_2 = row + 2;
        row_min_1 = row - 1;
        row_min_2 = row - 2; 
         }
 /*        
    const u16 pos =
        2 * bayer_img[row_min_1][col-1] +
        2 * bayer_img[row_min_1][col+1] +
        6 * bayer_img[row][col] +
        2 * bayer_img[row_add_1][col-1] +
        2 * bayer_img[row_add_1][col+1];
  */ 
        
   pos = 2 * bayer_img[row_min_1][col-1];
   wait();
   pos = pos + 2 * bayer_img[row_min_1][col+1];
   wait();
   pos = pos + 6 * bayer_img[row][col];
   wait();
   pos = pos +  2 * bayer_img[row_add_1][col-1];
   wait();
   pos = pos + 2 * bayer_img[row_add_1][col+1];
   
        wait();//scheduling memory
        
  neg = 3 * bayer_img[row_min_2][col];
  wait();
  neg = neg + 3 * bayer_img[row][col-2];
  wait();
  neg = neg + 3 * bayer_img[row][col+2];
  wait();
  neg = neg + 3 * bayer_img[row_add_2][col];
   
  /*      
    const u16 neg =
       (3 * bayer_img[row_min_2][col] +
        3 * bayer_img[row][col-2] +
        3 * bayer_img[row][col+2] +
        3 * bayer_img[row_add_2][col]);
  */  
    return compute_and_clamp_pixel_fractional_neg(pos, neg);
}


void debayer::wami_debayer(int cols, int img_row,
			   int cb_row, bool ping)
{
	/*
	 * In addition to what you did in part 1, you must
	 * update arrays indexes knowing that you only have
	 * a 6-rows circular buffer for the input and two ping
	 * pong memories, eahc of size 1 row for the output.
	 *
	 * HINT: since you only compute one row, what happens
	 *       to the outer loops over image rows??
	 */
  u32 col;
    //   for (row = PAD; row < rows-PAD; row += 2)
   // {
      
      cout << "img_row : " << img_row << " cb_row : " << cb_row << endl;
      
      if(!(img_row & 1)){//img_row %2 == 0){//should I use ping instead? ping == 0
        for (col = PAD; col < cols-PAD; col += 2)
        {
         if(ping == 0)
            debayer_img_pong[col-PAD].r = bayer_img[cb_row][col];       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].r = bayer_img[cb_row][col];  
            wait();
                                                                       //debayer_img[img_row-PAD][col-PAD]
        }                                                          
        //cout << "CHECK 1 " << endl;  
     
     }

    /* Copy top-right green pixels through directly */
    //for (row = PAD; row < rows-PAD; row += 2)
    //{
       if(!(img_row & 1)){//img_row%2 == 0){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].g = bayer_img[cb_row][col];       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].g = bayer_img[cb_row][col]; 
            wait();
        }
        //cout << "CHECK 2 " << endl;
    }
    

    /* Copy bottom-left green pixels through directly */
   // for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if((img_row & 1)){//img_row%2 == 1){//ping has no relationship with odd and even
        for (col = PAD; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].g = bayer_img[cb_row][col];       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].g = bayer_img[cb_row][col]; 
            wait();
        }
        //cout << "CHECK 3 " << endl;
    }

    /* Copy blue pixels through directly */
    //for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if((img_row & 1)){//img_row%2 == 1){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].b = bayer_img[cb_row][col];       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].b = bayer_img[cb_row][col]; 
            wait();
        }
        //cout << "CHECK 8 " << endl;
    }

    /* Interpolate green pixels at red pixels */
    //for (row = PAD; row < rows-PAD; row += 2)
    //{
       if(!(img_row & 1)){//img_row%2 == 0){
       
       //cout << "                img_row " << img_row << " ~(img_row & 1) " << ~(img_row & 1) << endl;
        for (col = PAD; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].g = interp_G_at_RRR_or_G_at_BBB(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].g = interp_G_at_RRR_or_G_at_BBB(cb_row,col); 
            wait();
            
        }
        //cout << "CHECK 4 " << endl;
    }
    

    /* Interpolate green pixels at blue pixels */
    //for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if(img_row & 1){//img_row%2 == 1){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].g = interp_G_at_RRR_or_G_at_BBB(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].g = interp_G_at_RRR_or_G_at_BBB(cb_row,col); 
            wait();
        }
        //cout << "CHECK 9 " << endl;
    }

    /* Interpolate red pixels at green pixels, red row, blue column */
    //for (row = PAD; row < rows-PAD; row += 2)
    //{
       if(~(img_row & 1)){//img_row%2 == 0){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].r = interp_R_at_GRB_or_B_at_GBR(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].r = interp_R_at_GRB_or_B_at_GBR(cb_row,col); 
            wait();
        }
        //cout << "CHECK 5 " << endl;
    }

    /* Interpolate blue pixels at green pixels, blue row, red column */
    //for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if(img_row & 1){//img_row%2 == 1){
        for (col = PAD; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].b = interp_R_at_GRB_or_B_at_GBR(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].b = interp_R_at_GRB_or_B_at_GBR(cb_row,col); 
            wait();
        }
        //cout << "CHECK 10 " << endl;
    }

    /* Interpolate red pixels at green pixels, blue row, red column */
    //for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if(img_row & 1){//img_row%2 == 1){
        for (col = PAD; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].r = interp_R_at_GBR_or_B_at_GRB(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].r = interp_R_at_GBR_or_B_at_GRB(cb_row,col); 
            wait();
        }
        //cout << "CHECK 11 " << endl;
    }
 
    /* Interpolate blue pixels at green pixels, red row, blue column */
   // for (row = PAD; row < rows-PAD; row += 2)
    //{
       if(!(img_row & 1)){//img_row%2 == 0){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].b = interp_R_at_GBR_or_B_at_GRB(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].b = interp_R_at_GBR_or_B_at_GRB(cb_row,col); 
            wait();
        }
        //cout << "CHECK 6 " << endl;
    }

    /* Interpolate red pixels at blue pixels, blue row, blue column */
    
    //for (row = PAD+1; row < rows-PAD; row += 2)
    //{
       if(img_row & 1){//img_row%2 == 1){
        for (col = PAD+1; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].r = interp_R_at_BBB_or_B_at_RRR(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].r = interp_R_at_BBB_or_B_at_RRR(cb_row,col);
            wait(); 
        }
        //cout << "CHECK 12 " << endl;
    }

    /* Interpolate blue pixels at red pixels, red row, red column */
    //for (row = PAD; row < rows-PAD; row += 2)
    //{
       if(!(img_row & 1)){//img_row%2 == 0){
        for (col = PAD; col < cols-PAD; col += 2)
        {
            if(ping == 0)
            debayer_img_pong[col-PAD].b = interp_R_at_BBB_or_B_at_RRR(cb_row,col);       //the ping/pong buffer is one dim it corresponds to *img_row-PAD* row
         else
            debayer_img_ping[col-PAD].b = interp_R_at_BBB_or_B_at_RRR(cb_row,col); 
            wait();
        }
        //cout << "CHECK 7 " << endl;
        
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
	}
}


void debayer::load_input()
{
RESET_LOAD:
	// Initialization at reset
  int pic_size = 0;
  int index_rd = 0;
  int index_bayer = 0;
  int row_counter_bayer = 0;
  
  input_done.write(false);
  bufdin.reset_get();
  
  rd_length.write(0);
  rd_index.write(0);
  
  wait();
  cout << "DUT load_input reset initial 3  " << sc_time_stamp() <<endl;
  
	// Wait for init_done from configuration and read image size
  do{ wait(); }
  while(!init_done.read());
  pic_size = size.read();
  rd_length.write(pic_size);

  
  //initialize Circular Buffer
  
  cout << "index_rd = " << index_rd << " pic_size = " << pic_size << endl;
  
  for(int k=0;k<4;k++){//one more burst inside
  
  cout << "k = " << k << endl;
  rd_request.write(true);//request high
  do {wait();}
  while(!rd_grant.read());//wait grant high
  
  rd_request.write(false);//request low
  do {wait();}
  while(rd_grant.read());//wait grant low
  
  
  for(int i=0;i<pic_size;i++)//transfer data
  {
   bayer_img[row_counter_bayer][i] = bufdin.get();//blocking
   //cout << "ring buffer row = " << index_rd/pic_size%CB_SIZE << endl;
   //cout << "bayer_img " <<  index_rd + i +1 << " = " << bayer_img[index_rd/pic_size%CB_SIZE][i] << endl;
   wait();
  }
   
   //cout << "buffer index is " << index_rd/pic_size << endl;
   index_rd = index_rd + rd_length.read();
   row_counter_bayer ++;
   cout << "rd_index is " << index_rd << endl;
   rd_index.write(index_rd);
  
  
  }

LOAD_INPUT_WHILE:
	while(true) {

		// If the entire image has been loaded wait for reset from TB
    //cout << "index_rd = " << index_rd << " pic_size = " << pic_size << endl;
    do{wait();}
    while(index_rd == pic_size*pic_size);

		// Send DMA request
		// Notice that this implementation is processing one row per burst!

		// 4-phase handshake load_input() with TB
    cout << "DUT send req to load @ " << sc_time_stamp() << endl;
    rd_request.write(true);//request high
    
    do {wait();}
    while(!rd_grant.read());//wait grant high
    
    rd_request.write(false);//request low
    do {wait();}
    while(rd_grant.read());//wait grant low
  
    index_bayer = row_counter_bayer;
  
    while(index_bayer > CB_SIZE - 1 ){
    index_bayer = index_bayer - CB_SIZE; 
    }
  
    for(int i=0;i<pic_size;i++){//transfer data
    bayer_img[index_bayer][i] = bufdin.get();//blocking
    //cout << "bayer_img " << index_rd + i+1 << " = " << bayer_img[index_rd/pic_size%CB_SIZE][i] << endl;
    wait();
    }
    cout << "buffer index is " << index_bayer << endl;
    index_rd = index_rd + rd_length.read();
    row_counter_bayer ++;
    cout << "rd_index is " << index_rd << endl;
    cout << "buffer index to be " << index_bayer << endl;
    rd_index.write(index_rd);
    
    cout << "DUT load_input done @ " << sc_time_stamp() << endl;
    
		// Loop over the entire array to get the image pixels
		// Make sure CtoS knows the maximum number of iterations, but
		// do not run useless iterations!
		// In addition, make sure you handle properly the circular buffer!

		// 4-phase handshake load_input() with process_debayer()
    
    input_done.write(true);
    do{wait();}
    while(!process_start.read());
    input_done.write(false);
  
	}
}


void debayer::process_debayer()
{
	// Initialization at reset
  int rows = 0;
  int pic_size = 0;
  int img_row = 0;
  int cb_row = 0;
  int index_rd = 0;
  bool ping = 0;
  int row_counter_debayer = 0;
  
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
    do{wait();}
    while(index_rd == pic_size*pic_size);
    
		// 4-phase handshake process_debayer() with load_input()
    do{wait();}
    while(!input_done.read());
    
    cout << "DUT receive req to process @ " << sc_time_stamp() << endl;
    
    process_start.write(true);
    
    index_rd = rd_index.read();//synchronizing point
    
    do{wait();}
    while(input_done.read());
    process_start.write(false);
		// Call wami_debayer(). See the header file for arguments description!
    rows = pic_size;
    
    //Now there 5 index in ring buffer
    img_row = row_counter_debayer;//???????????????????????????????? this is the index of debayered image
    
    cb_row = row_counter_debayer + 2;//??????????????????????????????????????
    
    while(cb_row > CB_SIZE-1){
    cb_row = cb_row - 6;
    }
    
    cout << "index : " << index_rd << " img_row : " << img_row << " cb_row : " 
         << cb_row << endl;
         
		wami_debayer(rows, img_row, cb_row, ping);
    ping = !ping;
    cout << "After process ping is " << ping << endl;
    
    row_counter_debayer ++;
   /**
	 * @cols: size of the image in terms of number of columns
	 * @img_row: current image row. Need to know wether it's odd or even
	 * @cb_row: footprint central row in the circular buffer
	 * @ping: flag to determne which will be the output buffer
	 */

		// 4-phase handshake process_debayer() with store_output()
    cout << "DUT process done @ " << sc_time_stamp() << endl;
     
    process_done.write(true);
    do{wait();}
    while(!output_start.read());
    process_done.write(false);
    
	}
}

void debayer::store_output()
{
	// Initialization at reset
  int pic_size = 0;
  int index_wr = 0;
  bool ping = 0;
  
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
  wr_length.write(pic_size-2*PAD);
  
STORE_OUTPUT_WHILE:
	while(true) {

		// If the entire image has been sent to TB
		// set debayer_done and wait for reset from TB
    if(index_wr == (pic_size-2*PAD) * (pic_size-2*PAD)) debayer_done.write(true);
    do{wait();}
    while(index_wr == (pic_size-2*PAD) * (pic_size-2*PAD));
    
    if(index_wr == (pic_size-2*PAD) * (pic_size-2*PAD)) debayer_done.write(true);
    
		// 4-phase handshake store_output() with process_debayer()
    do{wait();}
    while(!process_done.read());
    
    cout << "DUT receive req to store @ " << sc_time_stamp() << endl;
    //cout << "Row pic now at store is " << rd_index.read()/pic_size << endl;
    //cout << "Ring Buffer index is " << rd_index.read()/pic_size/CB_SIZE << endl;
    
    output_start.write(true);
    do{wait();}
    while(process_done.read());
    output_start.write(false);
    
		// Send DMA request

		// 4-phase handshake store_output() with TB
    wr_request.write(true);//request high
    do {wait();}
    while(!wr_grant.read());//wait grant high
    
    
    
    wr_request.write(false);//request low
    do {wait();}
    while(wr_grant.read());//wait grant low
    
    
    
    //cout << index_wr << endl;
    for(int j=0;j<pic_size-2*PAD;j++){//transfer data
    //cout << index_wr/(pic_size-2*PAD) << endl;
    
    //if(index_wr%(pic_size-2*PAD) == 1)
    if(ping == 1){
    bufdout.put(debayer_img_ping[j]);//blocking
    }
     
    //if(index_wr%(pic_size-2*PAD) == 0)
    if(ping == 0){
    bufdout.put(debayer_img_pong[j]);}
    }
    
    index_wr = index_wr + wr_length.read();
    wr_index.write(index_wr);
    
    //cout << "ROWS to be write : " << index_wr/(pic_size-2*PAD) << endl;
    ping = ! ping;
    cout << "After store ping is " << ping << endl;
    
    
    cout << "One burst of store done @ " << sc_time_stamp() << endl;
		// Loop over the entire array to send the output image pixels
		// Make sure CtoS knows the maximum number of iterations, but
		// do not run useless iterations!
		// In addition, alternate ping-pong buffers to store the output.
		// Ping-pong allows for concurrent output and computation.
   
	}
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif
