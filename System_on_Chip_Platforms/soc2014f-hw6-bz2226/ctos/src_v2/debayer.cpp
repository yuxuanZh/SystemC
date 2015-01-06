//Modified by Beinuo: remove ping-pong memory?
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


static char get_cb_row(char row)
{
	char cb_row = row;
	if (row >= CB_SIZE)
		cb_row = row - CB_SIZE;
	if (row < 0)
		cb_row = row + CB_SIZE;
	return cb_row;
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



void debayer::wami_debayer(u16 cols, u16 img_row,
			   char cb_row, bool ping)
{
	u16 col;
	char rown1,rown2,rowp1,rowp2;
	u16 coln1,coln2,colp1,colp2;
	u16 b_tmp0,b_tmp1,b_tmp2,b_tmp3,b_tmp4,b_tmp5,b_tmp6,b_tmp7,b_tmp8,b_tmp9,b_tmp10,b_tmp11,b_tmp12;
	    
      rown1 = get_cb_row(cb_row-1);
			rown2 = get_cb_row(cb_row-2);
			rowp1 = get_cb_row(cb_row+1);
			rowp2 = get_cb_row(cb_row+2);
			wait();
	
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 1)
		{
			if (col == cols-PAD)
				break;
			coln1 = col - 1;
			coln2 = col - 2;
			colp1 = col + 1;
			colp2 = col + 2;
		
      

      wait();
      b_tmp0 = bayer_img[rown2][col];
      b_tmp1 = bayer_img[rown1][coln1];
      b_tmp2 = bayer_img[rown1][col];
      b_tmp3 = bayer_img[rown1][colp1];
      b_tmp4 = bayer_img[cb_row][coln2];
      b_tmp5 = bayer_img[cb_row][coln1];
      b_tmp6 = bayer_img[cb_row][col];
      wait();
      b_tmp7 = bayer_img[cb_row][colp1];
      b_tmp8 = bayer_img[cb_row][colp2];
      b_tmp9 = bayer_img[rowp1][coln1];
      b_tmp10 = bayer_img[rowp1][col];
      b_tmp11 = bayer_img[rowp1][colp1];
      b_tmp12 = bayer_img[rowp2][col];
	
  	//u16 pix = bayer_img[cb_row][col];
		wait();
		wait();
    wait();
         u16 sum48 = b_tmp4 + b_tmp8;
         u16 sum012 = b_tmp0 + b_tmp12;
         u16  pos1 = 2*b_tmp2 + 2*b_tmp5 + 4*b_tmp6 + 2*b_tmp7+ 2*b_tmp10 ;
         //u16  neg1 = b_tmp0 + b_tmp4 + b_tmp8 + b_tmp12;
          u16 neg1 = sum48 + sum012;  
    u16 G_RRR_tmp = compute_and_clamp_pixel(pos1, neg1);
   
           u16  pos2 = 2*b_tmp1 + 2*b_tmp3 + 4*b_tmp6 + 2*b_tmp6 + 2*b_tmp9 + 2*b_tmp11;
          
           u16  neg2 = 2*neg1 + neg1;  
    u16 R_BBB_tmp = compute_and_clamp_pixel_fractional_neg(pos2, neg2);
           u16 neg34_tmp = b_tmp1 + b_tmp3 + b_tmp9 + b_tmp11;
           u16 pos3 = ((b_tmp4+b_tmp8)>>1) + 4*b_tmp2 + 4*b_tmp6 + b_tmp6 + 4*b_tmp10;
          
           u16 neg3 = neg34_tmp + sum012;
    u16 R_GBR_tmp = compute_and_clamp_pixel(pos3, neg3);  
   
           u16 pos4 = ((b_tmp0 + b_tmp12)>>1) + 4*b_tmp5 +4 *b_tmp6 + b_tmp6 + 4 * b_tmp7;
           
           u16 neg4 = neg34_tmp + sum48;
    u16 R_GRB_tmp =  compute_and_clamp_pixel(pos4, neg4);       

  //wait();
  if(!(img_row%2)^(col%2)){
   if(ping)      
   debayer_img_pingg[col-PAD] =G_RRR_tmp ;
   else
   debayer_img_pongg[col-PAD] =G_RRR_tmp ;
          }
   else
   if(ping)
   debayer_img_pingg[col-PAD] = b_tmp6;
   else
    debayer_img_pongg[col-PAD] = b_tmp6;
   
       wait();                     
		if (!(img_row % 2)) //even rows
		{

		   if(!(col%2)){
    			FOR_COPY_RED:
        if(ping)  
				debayer_img_pingr[col-PAD] = b_tmp6;
        else
        debayer_img_pongr[col-PAD] = b_tmp6;
      
   			FOR_INTERP_GR:  
				//debayer_img_g[col-PAD] = interp_G_at_RRR_or_G_at_BBB(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
 
        
   			FOR_INTERP_BR_RR_RC:
      				if(ping)
              debayer_img_pingb[col-PAD] = R_BBB_tmp;//interp_R_at_BBB_or_B_at_RRR(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
              else
              debayer_img_pongb[col-PAD] = R_BBB_tmp;
     
  		    }// if

		   else{
    			FOR_COPY_GREEN_TR:
				//debayer_img_g[col-PAD] = pix;
        
  		        FOR_INTERP_RG_RR_BC:  
        if(ping)        
				debayer_img_pingr[col-PAD] = R_GRB_tmp;//interp_R_at_GRB_or_B_at_GBR(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
        else
        debayer_img_pongr[col-PAD] = R_GRB_tmp;
      		
  		        FOR_INTERP_BG_RR_BC:
            if(ping)
      				debayer_img_pingb[col-PAD] = R_GBR_tmp;//interp_R_at_GBR_or_B_at_GRB(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
            else
              debayer_img_pongb[col-PAD] = R_GBR_tmp;
		  }//else
                }//if//even rows       
   
	

	      else//if (img_row & 2) //odd rows
	      {

		   if(!(col%2)){
    			FOR_COPY_GREEN_BL:
				//debayer_img_g[col-PAD] = pix;
        
    			FOR_INTERP_BG_BR_RC:
        if(ping)  
				debayer_img_pingb[col-PAD] = R_GRB_tmp;//interp_R_at_GRB_or_B_at_GBR(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
        else
        debayer_img_pongb[col-PAD] = R_GRB_tmp;
        
   		        FOR_INTERP_RG_BR_RC:
        if(ping)
				debayer_img_pingr[col-PAD] = R_GBR_tmp;//interp_R_at_GBR_or_B_at_GRB(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
        else
        debayer_img_pongr[col-PAD] = R_GBR_tmp;
		
   		    }//if
   

                    else{
    			FOR_COPY_BLUE:
        if(ping)
				debayer_img_pingb[col-PAD] = b_tmp6;
        else
        debayer_img_pongb[col-PAD] = b_tmp6;
        
   			FOR_INTERP_GB:   
				//debayer_img_g[col-PAD] = interp_G_at_RRR_vi tor_G_at_BBB(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
        
  			FOR_INTERP_RB_BR_BC:
        if(ping)
				debayer_img_pingr[col-PAD] =  R_BBB_tmp;//interp_R_at_BBB_or_B_at_RRR(cb_row, rown1,rown2,rowp1,rowp2,col,coln1,coln2,colp1,colp2);
        else
        debayer_img_pongr[col-PAD] =  R_BBB_tmp;
		
			}//else
		
	      }//else //odd rows
	}//for
}

void debayer::config_debayer() 
{
	// Initialization
	size.write(0);
	init_done.write(false);

	wait();

	// Read configuration until done
	bool done = false;
CONFIG_REGISTER_WHILE:
	do {
		wait();
		done = conf_done.read();
		int a = conf_size.read();
		size.write(a);
	} while (!done);

	// Let other threads run then do nothing
	init_done.write(true);
	while (true) {
		wait();
	}
}


void debayer::load_input()
{
RESET_LOAD:
	bufdin.reset_get();

	rd_index.write(0);
	rd_length.write(0);
	rd_request.write(false);

	input_done.write(false);

	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();

	int index = 0;
	// Use flag to fill in the buffer with the first five rows
	
	bool cold_start = true;
	u8 cb_row = 0;
  u16 length = 4*rows  + rows;
  u16 cnt = 4;
LOAD_INPUT_WHILE:
	while(true) {
		if (cnt == rows)
			// Input complete; wait for reset
			do { wait(); }	while (true);
     
         
		// Send DMA request
		if (!cold_start) length = rows;
	  rd_index.write(index);
    rd_length.write(length);
		// This implementation processes one row at a time
		index += length;
		cnt++;
		// 4-phase handshake
		rd_request.write(true);
		do { wait(); } while(!rd_grant.read());
		rd_request.write(false);
		
		//LOAD_INIT_LOOP:
			LOAD_INIT_INNER_LOOP:
      
      for (u16 j = 0; j < IMG_NUM_COLS; j++) {
      if (j == rows){
         cb_row += 1;
        if(!cold_start){
            {wait();break;}
           }
        else
        {
           j = 0; 
           wait(); 
           if(cb_row == 5) {cold_start = false;{wait();break;}}
           }}
        bayer_img[cb_row][j] = bufdin.get();
        wait();
          }
      
     
      
      

		// Do not overflow the circular buffer
   wait();
		if (cb_row == CB_SIZE)
			cb_row = 0;
		// 4-phase handshake
		input_done.write(true);
		do { wait(); }
		while (!process_start.read());
		input_done.write(false);
		do { wait(); }
		while (process_start.read());

	}
}



void debayer::process_debayer()
{
	// Reset
	process_start.write(false);
	process_done.write(false);

	// Wait for configuratin
	do { wait(); } while (!init_done.read());

	const u16 rows = size.read();
	char cb_row = PAD;
	u16 img_row = PAD;
	bool ping = true;

DEBAYER_WHILE:
	while (true) {

		if (img_row == rows - PAD)
			// Wait for reset
			do { wait(); } while(true);

		// 4-phase handshake
		do { wait(); }
		while (!input_done.read());
		process_start.write(true);
		do { wait(); }
		while (input_done.read());
		process_start.write(false);

		wami_debayer(rows, img_row, cb_row,ping);

		// 4-phase handshake
		process_done.write(true);
		do { wait(); }
		while (!output_start.read());
		process_done.write(false);
		do { wait(); }
		while (output_start.read());

		/**
		 * We compute one output row per iteration
		 */
		img_row++;
		cb_row++;
		if (cb_row == CB_SIZE)
			cb_row = 0;
		ping = !ping;
		//do {wait();}
		//while(output_busy.read());
	}
}

void debayer::store_output()
{
RESET_STORE:
	bufdout.reset_put();

	wr_index.write(0);
	wr_request.write(false);
	wr_length.write(0);

	output_start.write(false);
	debayer_done.write(false);
	//output_busy.write(false);

	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read() - (2 * PAD);
	int index = 0;
  u16 cnt = 0;
	//u16 length = rows;??
	bool ping = true;

STORE_OUTPUT_WHILE:
	while(true) {

		if (cnt == rows) {
			// DEBAYER Done (need a reset)
			debayer_done.write(true);
			do { wait(); } while(true);
		}
    cnt++;

		// 4-phase handshake
		do { wait(); }
		while (!process_done.read());
		output_start.write(true);
		//output_busy.write(true);
		do { wait(); }
		while (process_done.read());
		output_start.write(false);

		// Send DMA request
		wr_index.write(index);
		wr_length.write(rows);
		index += rows;

		// 4-phase handshake
		wr_request.write(true);
		do { wait(); } while(!wr_grant.read());
		wr_request.write(false);
		
	STORE_SINGLE_LOOP:
		for (u16 j = 0; j < IMG_NUM_COLS - 2 * PAD; j++) {
			if (j == rows)
				break;
		//	u16 pixr,pixg,pixb;
			u16 pixr,pixb,pixg;
      
     if(ping){
      pixr = debayer_img_pingr[j];
      pixg = debayer_img_pingg[j];
      pixb = debayer_img_pingb[j];   
      }
      else{
      pixr = debayer_img_pongr[j];
      pixg = debayer_img_pongg[j];
      pixb = debayer_img_pongb[j];  
      }
      wait();
      //pix = debayer_img_r[j];
			wait();
			bufdout.put(pixr);
     // pix = debayer_img_g[j];
			wait();
			bufdout.put(pixg);
     // pix = debayer_img_b[j];
			wait();
			bufdout.put(pixb);

		}
		ping = !ping;
		//output_busy.write(false);
	}
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif
