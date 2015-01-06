//modified by Beinuo @ 11/27
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


static int get_cb_row(char row)
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
u16 debayer::interp_G_at_RRR_or_G_at_BBB(u16 row, u32 col)
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
	u16 pos, neg, tmp1,tmp2,tmp3,tmp4,tmp5,tmp7,tmp6,tmp8,p_tmp1,p_tmp2,p_tmp3,n_tmp1,n_tmp2;
  //
  wait();
  //
	tmp1 = bayer_img1[get_cb_row(row-1)][col];
	tmp2 = bayer_img2[get_cb_row(row)][col+2];
	tmp5 = bayer_img3[get_cb_row(row)][col];
	tmp6 = bayer_img3[get_cb_row(row)][col-2];
	wait();
	tmp3 = bayer_img1[get_cb_row(row)][col-1];
	tmp4 = bayer_img2[get_cb_row(row-2)][col];
	tmp7 = bayer_img3[get_cb_row(row)][col+1];
	tmp8 = bayer_img3[get_cb_row(row+2)][col];
  wait();
	p_tmp1 = 2 * tmp1 + 4 * tmp5;
	n_tmp1 = tmp2 + tmp6;

	tmp1 = bayer_img3[get_cb_row(row+1)][col];
  wait();
	p_tmp2 = 2 * tmp3 + 2 * tmp7;
	n_tmp2 = tmp4 + tmp8;	
  wait();
		
	p_tmp3 = p_tmp1 + p_tmp2;
	neg = n_tmp1 + n_tmp2;
	pos = 2 * tmp1 + p_tmp3;
	//pos += 4 * tmp1;
	//neg += tmp2;
	//tmp1 = bayer_img1[get_cb_row(row+1)][col];
  wait();
	//pos += 2 * tmp3;
	//neg += tmp4;
	
	//wait();
	//pos +=	2 * tmp1;

	//wait();

//

	//tmp6 = bayer_img1[get_cb_row(row)][col+2];
	//neg = tmp;
	//tmp7 = bayer_img1[get_cb_row(row-2)][col];
	//neg += tmp;
	//tmp8 = bayer_img1[get_cb_row(row)][col-2];
	//neg += tmp;
	//tmp9= bayer_img1[get_cb_row(row+2)][col];
	//neg += tmp;


	return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_GRB_or_B_at_GBR(u16 row, u32 col)
{
	/*
	 * [0  0 0.5 0  0
	 *  0 -1  0 -1  0
	 * -1  4  5  4 -1
	 *  0 -1  0 -1  0
	 *  0  0 0.5 0  0]/8;
	 */
	u16 pos, neg, tmp,tmp1,tmp2, tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,tmp9,tmp10,p_tmp1,p_tmp2,n_tmp1,n_tmp2,n_tmp3;
 //
  wait();
  //
	tmp1 = bayer_img1[get_cb_row(row+2)][col];
	tmp2 = bayer_img2[get_cb_row(row-1)][col-1];
	tmp5 = bayer_img3[get_cb_row(row-2)][col];
	tmp6 = bayer_img3[get_cb_row(row-1)][col+1];
	wait();
	//tmp3 = bayer_img1[get_cb_row(row-2)][col];5
	//tmp4 = bayer_img2[get_cb_row(row-1)][col+1];6
	tmp3 = bayer_img1[get_cb_row(row)][col-1];
	tmp4 = bayer_img2[get_cb_row(row)][col+2];
	tmp7 = bayer_img3[get_cb_row(row)][col];
	tmp8 = bayer_img3[get_cb_row(row)][col-2];
  wait();
 	tmp = ((tmp1 + tmp5) >>1);
	n_tmp1 = tmp2 + tmp6;
	//tmp2 = bayer_img2[get_cb_row(row)][col-2];8
	tmp1 = bayer_img3[get_cb_row(row)][col+1];
	tmp2= bayer_img3[get_cb_row(row+1)][col+1];
	tmp10 = bayer_img2[get_cb_row(row+1)][col-1];
  wait();
	p_tmp1 = tmp + 4 * tmp3;
	n_tmp2 = tmp4 + tmp8;	
	//tmp = ((tmp1 + tmp3) >>1);
	//neg += tmp4;
	//tmp1 = bayer_img1[get_cb_row(row)][col-1];3
	//tmp4 = bayer_img2[get_cb_row(row)][col+2];4

  wait();
	p_tmp2 = 5 * tmp7 + 4 * tmp1;
	n_tmp3 = tmp10 + tmp2;
	neg = n_tmp1 + n_tmp2;
	//neg += tmp2;
	//tmp3 = bayer_img1[get_cb_row(row)][col];7
	//tmp2 = bayer_img2[get_cb_row(row+1)][col-1];10
  wait();
	pos = p_tmp1 + p_tmp2;	
	neg += n_tmp3;
	//pos += 4 * tmp1;
	//neg += tmp4;
	//tmp1 = bayer_img1[get_cb_row(row)][col+1];1
	//tmp4= bayer_img2[get_cb_row(row+1)][col+1];2
  wait();
	//pos += 5 * tmp3;
	//neg += tmp2;

 // wait();
	//pos += 4 * tmp1;
	//neg += tmp4;
 //
 // wait();

	//tmp6 = bayer_img3[get_cb_row(row-1)][col-1];
	//neg = tmp;
	//tmp7 = bayer_img3[get_cb_row(row-1)][col+1];
	//neg += tmp;
	//tmp8 = bayer_img3[get_cb_row(row)][col-2];
	//neg += tmp;
	//tmp9 = bayer_img5[get_cb_row(row)][col+2];
	//neg += tmp;
	//tmp10 = bayer_img5[get_cb_row(row+1)][col-1];
	//neg += tmp;
	//tmp11 = bayer_img5[get_cb_row(row+1)][col+1];
	//neg += tmp;

	
	
	return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_GBR_or_B_at_GRB(u16 row,  u32 col)
{
	/*
	 * [0  0 -1  0  0
	 *  0 -1  4 -1  0
	 * 0.5 0  5  0 0.5
	 *  0 -1  4 -1  0
	 *  0  0 -1 0  0]/8;
	 */
	u16 pos, neg, tmp,tmp1,tmp2, tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,tmp9,tmp10,p_tmp1,p_tmp2,n_tmp1,n_tmp2,n_tmp3;
 //
  wait();
  //
	tmp1 = bayer_img1[get_cb_row(row)][col-2];
	tmp2 = bayer_img2[get_cb_row(row-2)][col];
  	tmp5 = bayer_img3[get_cb_row(row)][col+2];
	tmp6 = bayer_img3[get_cb_row(row-1)][col-1];
 wait();
  	//tmp3 = bayer_img1[get_cb_row(row)][col+2];5
	//tmp4 = bayer_img2[get_cb_row(row-1)][col-1];6
	tmp3 = bayer_img1[get_cb_row(row-1)][col];
	tmp4 = bayer_img2[get_cb_row(row+1)][col-1];
	tmp7 = bayer_img3[get_cb_row(row)][col];
	tmp8 = bayer_img3[get_cb_row(row-1)][col+1];
  wait();
	tmp = ((tmp1+tmp5)>>1);
	n_tmp1 = tmp2 + tmp6;
	//tmp2 = bayer_img2[get_cb_row(row-1)][col+1];8
	tmp1 = bayer_img3[get_cb_row(row+1)][col];
	tmp2 = bayer_img3[get_cb_row(row+2)][col];
	tmp10 =bayer_img1[get_cb_row(row+1)][col+1];
  wait();
	n_tmp2 = tmp4 + tmp8;
	p_tmp1 =tmp + 4 * tmp3;	
	//tmp = ((tmp1+tmp3)>>1);
	//neg += tmp4;
	//tmp1 = bayer_img1[get_cb_row(row-1)][col];3
	//tmp4 = bayer_img2[get_cb_row(row+1)][col-1];4
  wait();
	n_tmp3 = tmp2 + tmp10;
	p_tmp2 = 5 * tmp7 + 4 * tmp1;
	neg =n_tmp2 + n_tmp1;
	//pos = tmp;
	//neg += tmp2;
	//tmp3 = bayer_img1[get_cb_row(row)][col];7
	//tmp2 = bayer_img2[get_cb_row(row+1)][col+1];10
  wait();
	pos = p_tmp1 + p_tmp2;
	neg += n_tmp3;
	//pos += 4 * tmp1;
	//neg += tmp4;
	//tmp1 = bayer_img1[get_cb_row(row+1)][col];1
	//tmp4 = bayer_img2[get_cb_row(row+2)][col];2
  //wait();
	//pos += 5 * tmp3;
	//neg += tmp2;

  //wait();
	//pos += 4 * tmp1;
	//neg += tmp4;
 //
  wait();
	//tmp6 = bayer_img4[get_cb_row(row-2)][col];
	//neg = tmp;
	//tmp7 = bayer_img4[get_cb_row(row-1)][col-1];
	//neg += tmp;
	//tmp8 = bayer_img4[get_cb_row(row-1)][col+1];
	//neg += tmp;
	//tmp9 = bayer_img5[get_cb_row(row+1)][col-1];
	//neg += tmp;
	//tmp10 = bayer_img5[get_cb_row(row+1)][col+1];
	//neg += tmp;
	//tmp11 = bayer_img5[get_cb_row(row+2)][col];
	//neg += tmp;



	return compute_and_clamp_pixel(pos, neg);
}

u16 debayer::interp_R_at_BBB_or_B_at_RRR(u16 row, u32 col)
{
	/*
	 * [0  0 -1.5 0  0
	 *  0  2  0  2  0
	 * -1.5 0  6  0 -1.5
	 *  0  2  0  2  0
	 *  0  0 -1.5 0  0]/8;
	 */
	u16 pos, neg, tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8,p_tmp1,p_tmp2,p_tmp3,n_tmp1,n_tmp2;
 //
  wait();
  //
	tmp1 = bayer_img1[get_cb_row(row-1)][col-1];
	tmp2 = bayer_img2[get_cb_row(row-2)][col];
	tmp5 = bayer_img3[get_cb_row(row)][col];
	tmp6 = bayer_img3[get_cb_row(row)][col+2];
  wait();
 	tmp3 = bayer_img1[get_cb_row(row-1)][col+1];
	tmp4 = bayer_img2[get_cb_row(row)][col-2];
	tmp7 = bayer_img3[get_cb_row(row+1)][col-1];
	tmp8 = bayer_img3[get_cb_row(row+2)][col];
  wait();
	p_tmp1 = 2 * tmp1 + 6 * tmp5;;
	n_tmp1 = 3 * tmp2 + 3 * tmp6;
	//tmp1 = bayer_img1[get_cb_row(row)][col];5
	//tmp2 = bayer_img2[get_cb_row(row)][col+2];6
	tmp1 = bayer_img3[get_cb_row(row+1)][col+1];
  wait();
	p_tmp2= 2 * tmp3 + 2 *tmp7;
	n_tmp2 = 3 * tmp4 + 3* tmp8;
	//tmp3 = bayer_img1[get_cb_row(row+1)][col-1];7
	//tmp4 = bayer_img2[get_cb_row(row+2)][col];8
  wait();
	neg = n_tmp1 + n_tmp2;
	p_tmp3 = p_tmp1 + p_tmp2;
	pos = p_tmp3 + 2 * tmp1;	
	//pos += 6 * tmp1;
	//neg += 3 * tmp2;
	//tmp1 = bayer_img1[get_cb_row(row+1)][col+1];
  //wait();
	//pos += 2 * tmp3;
	//neg += 3 * tmp4;

  //wait();
	//pos += 2 * tmp1;
//
wait();

	//tmp6 = bayer_img2[get_cb_row(row-2)][col];
	//neg = 3 * tmp;
	//tmp7 = bayer_img2[get_cb_row(row)][col-2];
	//neg += 3 * tmp;
	//tmp8 = bayer_img2[get_cb_row(row)][col+2];
	//neg += 3 * tmp;
	//tmp9 = bayer_img2[get_cb_row(row+2)][col];
	//neg += 3 * tmp;



	return compute_and_clamp_pixel_fractional_neg(pos, neg);
}

//********************for loop1**************************//
void debayer::wami_debayer1(u16 cols, int img_row,
			   int cb_row, bool ping)
{
	u16 col;

	/* Copy red pixels through directly */
	//if (!(img_row % 2)) //even rows		//loop1
//	{
	FOR_COPY_RED:
		for (col = PAD; col < cols-PAD; col += 1)
		{
   wait();
 if(!(col%2)){
	if (!(img_row % 2)) //even rows		//loop1
	{
			u16 pix = bayer_img1[cb_row][col];
			// Need 1 cycle to ready from scratchpad memory
			wait();
      //
      wait();
      //
      /**************************/
			// Choose output buffer
			if (ping)
				//debayer_img_ping[col-PAD].r = pix;
				debayer_img_pingr[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].r = pix;
				debayer_img_pongr[col-PAD] = pix;
		}
	else{//odd rows	//loop1

	/* Copy bottom-left green pixels through directly */
	//if (img_row % 2) //odd rows	//loop1
//	{
	FOR_COPY_GREEN_BL:
		//for (col = PAD; col < cols-PAD; col += 2)
	//	{
			//if (col >= cols-PAD)
			//	break;
			u16 pix = bayer_img2[cb_row][col];
			wait();
      //
      wait();
      //
			if (ping)
				//debayer_img_ping[col-PAD].g = pix;
				debayer_img_pingg[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].g = pix;
				debayer_img_pongg[col-PAD] = pix;
		}
	}
 else{ //(col %2 ==1)
  	if (!(img_row % 2))
	  {
			//if (col >= cols-PAD)
			//	break;
			u16 pix = bayer_img2[cb_row][col];
			wait();
      //
      wait();
      //
			if (ping)
				//debayer_img_ping[col-PAD].g = pix;
				debayer_img_pingg[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].g = pix;
				debayer_img_pongg[col-PAD] = pix;
		}
	


	/* Copy blue pixels through directly */		//loop2
	//if (img_row % 2)
   else
	{
	FOR_COPY_BLUE:
	//	for (col = PAD+1; col < cols-PAD; col += 2)
	//	{
			//if (col >= cols-PAD)
			//	break;
			u16 pix = bayer_img1[cb_row][col];
			wait();
      //
      wait();
      //
			if (ping)
				//debayer_img_ping[col-PAD].b = pix;
				debayer_img_pingb[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].b = pix;
				debayer_img_pongb[col-PAD] = pix;
		}
  
  }//else
 }//for
}

//********************for loop2**************************//


//********************for loop3**************************//
void debayer::wami_debayer3(u16 cols, int img_row,
			   int cb_row, bool ping)
{
	u16 col;
	/* Interpolate green pixels at red pixels */	//loop3
	//if (!(img_row % 2))
	//{

	FOR_INTERP_GR:
		for (col = PAD; col < cols-PAD; col += 1)
		{wait();
    if(!(img_row % 2)^(col%2)){
			//if (col >= cols-PAD)
				//break;
        
			u16 pix = interp_G_at_RRR_or_G_at_BBB(cb_row, col);
			if (ping)
				//debayer_img_ping[col-PAD].g = pix;
				debayer_img_pingg[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].g = pix;
				debayer_img_pongg[col-PAD] = pix;
        }
		}
	//}


	/* Interpolate green pixels at blue pixels */	//loop3
	//if (img_row % 2)
	//{

//}
}


//********************for loop4**************************//
void debayer::wami_debayer4(u16 cols, int img_row,
			   int cb_row, bool ping)
{
	u16 col,coln;
 
	/* Interpolate red pixels at green pixels, red row, blue column */	//loop4
//	if (!(img_row % 2))
//	{
	FOR_INTERP_RG_RR_BC:
		
    for (col = PAD; col < cols-PAD; col += 2)
		{  
		
      coln = col+1;
    	if ((!(img_row % 2)) )
	{	 wait();
    u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, coln);
		//	if (col >= cols-PAD)
		//		break;/
 //  wait();
	//		u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, col);
			
			if (ping)
				//debayer_img_ping[col-PAD].r = pix;
				debayer_img_pingr[coln-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].r = pix;
				debayer_img_pongr[coln-PAD] = pix;
		}
//	}

	/* Interpolate blue pixels at green pixels, blue row, red column */		//loop4
//	if (img_row % 2)
	//{
//	FOR_INTERP_BG_BR_RC:
	//	for (col = PAD; col < cols-PAD; col += 2)
	//	{
   	else{
      wait();
     	u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, col);
			//if (col >= cols-PAD)
			//	break;
    //  wait();
			//u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, col);
			
			if (ping)
				//debayer_img_ping[col-PAD].b = pix;
				debayer_img_pingb[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].b = pix;
				debayer_img_pongb[col-PAD] = pix;
		}
  
	}
}


//********************for loop5**************************//
void debayer::wami_debayer5(u16 cols, int img_row,
			   int cb_row, bool ping)
{	
	u16 col;
	/* Interpolate blue pixels at green pixels, red row, blue column */		//loop5
//	if (!(img_row % 2))
//	{
	FOR_INTERP_BG_RR_BC:
		for (col = PAD; col < cols-PAD; col += 2)
		{//cout <<"@@@"<<sc_time_stamp()<<" @@@" << col <<endl;
			//if (col >= cols-PAD)
			//	break;
      u16 coln = col + 1;
      if (!(img_row % 2))
	{
      wait();
			u16 pix = interp_R_at_GBR_or_B_at_GRB(cb_row, coln);
			
			if (ping)
				//debayer_img_ping[col-PAD].b = pix;
				debayer_img_pingb[coln-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].b = pix;
				debayer_img_pongb[coln-PAD] = pix;
		}
	//}

	/* Interpolate red pixels at green pixels, blue row, red column */		//loop5
//	if (img_row % 2)
//	{
	//FOR_INTERP_RG_BR_RC:
	//	for (col = PAD; col < cols-PAD; col += 2)
//		{
			//if (col >= cols-PAD)
			//	break;
      	else
	{
      wait();
			u16 pix = interp_R_at_GBR_or_B_at_GRB(cb_row, col);
			
			if (ping)
				//debayer_img_ping[col-PAD].r = pix;
				debayer_img_pingr[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].r = pix;
				debayer_img_pongr[col-PAD] = pix;
		}
	}

}

//********************for loop6**************************//
void debayer::wami_debayer6(u16 cols, int img_row,
			   int cb_row, bool ping)
{
	u16 col, coln;
	/* Interpolate blue pixels at red pixels, red row, red column */		//loop6
//	if (!(img_row % 2))
//	{
	FOR_INTERP_BR_RR_RC:
		for (col = PAD; col < cols-PAD; col += 2)
		{
			//if (col >= cols-PAD)
			//	break;
      coln = col + 1;
      	if (!(img_row % 2))
	{
      wait();
			u16 pix = interp_R_at_BBB_or_B_at_RRR(cb_row, col);
			
			if (ping)
				//debayer_img_ping[col-PAD].b = pix;
				debayer_img_pingb[col-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].b = pix;
				debayer_img_pongb[col-PAD] = pix;
		}
	//}


	/* Interpolate red pixels at blue pixels, blue row, blue column */		//loop6
	//if (img_row % 2)
//	{
//	FOR_INTERP_RB_BR_BC:
//		for (col = PAD+1; col < cols-PAD; col += 2)
	//	{
			//if (col >= cols-PAD)
				//break;
        	else
	{
        wait();
			u16 pix = interp_R_at_BBB_or_B_at_RRR(cb_row, coln);
			
			if (ping)
				//debayer_img_ping[col-PAD].r = pix;
				debayer_img_pingr[coln-PAD] = pix;
			else
				//debayer_img_pong[col-PAD].r = pix;
				debayer_img_pongr[coln-PAD] = pix;
		}
	}
}



//*****************************LOOP1*******************************//
void debayer::debayer_loop1()
{
	//Initialization
	loop1_busy.write(false);
	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();
	char cb_row = cb_row_s.read();
	u16 img_row = img_row_s.read();
	bool ping =ping_s.read();
	

while(true){
	do{wait();}
	while(!start_loop.read());
	
	//begin debayer
	cb_row = cb_row_s.read();
	img_row = img_row_s.read();
	ping =ping_s.read();
	loop1_busy.write(true);
		wami_debayer1(rows, img_row, cb_row, ping);
	loop1_busy.write(false);
		wait();
}

}
//*****************************LOOP2*******************************//

//*****************************LOOP3*******************************//
void debayer::debayer_loop3()
{
	//Initialization
	loop3_busy.write(false);
	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();
	char cb_row = cb_row_s.read();
	u16 img_row = img_row_s.read();
	bool ping =ping_s.read();
	

while(true){
	do{wait();}
	while(!start_loop.read());
	
	//begin debayer
	cb_row = cb_row_s.read();
	img_row = img_row_s.read();
	ping =ping_s.read();
	loop3_busy.write(true);
		wami_debayer3(rows, img_row, cb_row, ping);
	loop3_busy.write(false);
		wait();
}

}
//*****************************LOOP4*******************************//
void debayer::debayer_loop4()
{
	//Initialization
	loop4_busy.write(false);
	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();
	char cb_row = cb_row_s.read();
	u16 img_row = img_row_s.read();
	bool ping =ping_s.read();
	

while(true){
	do{wait();}
	while(!start_loop.read());
	
	//begin debayer
	cb_row = cb_row_s.read();
	img_row = img_row_s.read();
	ping =ping_s.read();
	loop4_busy.write(true);
		wami_debayer4(rows, img_row, cb_row, ping);
	loop4_busy.write(false);
		wait();
}

}
//*****************************LOOP5*******************************//
void debayer::debayer_loop5()
{
	//Initialization
	loop5_busy.write(false);
	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();
	char cb_row = cb_row_s.read();
	u16 img_row = img_row_s.read();
	bool ping =ping_s.read();
	

while(true){
	do{wait();}
	while(!start_loop.read());
	//begin debayer
	cb_row = cb_row_s.read();
	img_row = img_row_s.read();
	ping =ping_s.read();
	loop5_busy.write(true);
		wami_debayer5(rows, img_row, cb_row, ping);
	loop5_busy.write(false);
		wait();
}

}
//*****************************LOOP6*******************************//
void debayer::debayer_loop6()
{
	//Initialization
	loop6_busy.write(false);
	do {wait();}
	while(!init_done.read());

	const u16 rows = size.read();
	char cb_row = cb_row_s.read();
	u16 img_row = img_row_s.read();
	bool ping =ping_s.read();
	

while(true){
	do{wait();}
	while(!start_loop.read());
	
	//begin debayer
	cb_row = cb_row_s.read();
	img_row = img_row_s.read();
	ping =ping_s.read();
	loop6_busy.write(true);
		wami_debayer6(rows, img_row, cb_row, ping);
	loop6_busy.write(false);
		wait();
}

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
	u16 length = 4 * rows + rows;
	bool cold_start = true;
	u8 cb_row = 0;
  u16 img_row = 4;
	wait();
LOAD_INPUT_WHILE:
	while(true) {

		//if (index == rows * rows)
    if(img_row == rows )
			// Input complete; wait for reset
			do { wait(); }	while (true);

		// Send DMA request
		if (!cold_start) length = rows; 
		rd_index.write(index);
		rd_length.write(length);
		// This implementation processes one row at a time
		index += length;
   img_row ++;
  wait();///new
		// 4-phase handshake
		rd_request.write(true);
		do { wait(); } while(!rd_grant.read());
		rd_request.write(false);

		
		LOAD_INIT_LOOP:
		
			
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
        u16 tmp = bufdin.get();
        bayer_img1[cb_row][j] = tmp;
        bayer_img2[cb_row][j] = tmp;
        bayer_img3[cb_row][j] = tmp;
        wait();
          }
		

		// Do not overflow the circular buffer
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
	start_loop.write(false);


	// Wait for configuratin
	do { wait(); } while (!init_done.read());

	const u16 rows = size.read();
	char cb_row = PAD;
	u16 img_row = PAD;
	bool ping = true;
	wait();
		img_row_s.write(img_row);
		cb_row_s.write(cb_row);
		ping_s.write(ping);
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
//cout << "debayer begain for row:" << img_row << "@ " << sc_time_stamp << endl;
		start_loop.write(true);
	
	
		do {wait();}
		while(!loop1_busy.read()  && !loop3_busy.read() && !loop4_busy.read() && !loop5_busy.read() && !loop6_busy.read());
		start_loop.write(false);
		

		do {wait();}
		while(loop1_busy.read()  || loop3_busy.read() || loop4_busy.read() || loop5_busy.read() || loop6_busy.read());
//cout << "debayer done for row:" << img_row << "@ " << sc_time_stamp() << endl;
		
	//	wami_debayer(rows, img_row, cb_row, ping);
		

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

		img_row_s.write(img_row);
		cb_row_s.write(cb_row);
		ping_s.write(ping);
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

	do {wait();}
	while(!init_done.read());
	wait();
	const u16 rows = size.read() - (2 * PAD);
	int index = 0;
	u16 length = rows;
  u16 img_row;
	bool ping = true;
  wait();
STORE_OUTPUT_WHILE:
	while(true) {
         img_row = img_row_s.read();
         wait();
      if(img_row == rows + PAD)
		//if (index == rows * rows) 
    {
			// DEBAYER Done (need a reset)
			debayer_done.write(true);
			do { wait(); } while(true);
		}

		// 4-phase handshake
		do { wait(); }
		while (!process_done.read());
		output_start.write(true);
		do { wait(); }
		while (process_done.read());
		output_start.write(false);

		// Send DMA request
		wr_index.write(index);
		wr_length.write(length);
		index += length;
	u16 pixr_c, pixg_c, pixb_c,pixr_n,pixg_n,pixb_n;
      if(ping){
      pixr_n = debayer_img_pingr[0];
      pixg_n = debayer_img_pingg[0];
      pixb_n = debayer_img_pingb[0];
      }
      else{
      pixr_n = debayer_img_pongr[0];
      pixg_n = debayer_img_pongg[0];
      pixb_n = debayer_img_pongb[0];
      }      
      wait();
		// 4-phase handshake
		wr_request.write(true);
		do { wait(); } while(!wr_grant.read());
		wr_request.write(false);



	STORE_SINGLE_LOOP:
		for (short int j = 0; j < IMG_NUM_COLS - 2 * PAD; j++) {
			if (j == rows){
				wait();
				break;}
		
      if (ping){
				//pix = debayer_img_ping[j];
      pixr_c = pixr_n;
      pixg_c = pixg_n;
      pixb_c = pixb_n;
      pixr_n = debayer_img_pingr[j+1];
      pixg_n = debayer_img_pingg[j+1];
      pixb_n = debayer_img_pingb[j+1];   

      //
  //    wait();
      //
			wait();
			//bufdout.put(pix.r);
			bufdout.put(pixr_c);
			wait();
			//bufdout.put(pix.g);
			bufdout.put(pixg_c);
			wait();

			//bufdout.put(pix.b);
			bufdout.put(pixb_c);


			}
			else{
				//pix = debayer_img_pong[j];
      pixr_c = pixr_n;
      pixg_c = pixg_n;
      pixb_c = pixb_n;
      pixr_n = debayer_img_pongr[j+1];
      pixg_n = debayer_img_pongg[j+1];
      pixb_n = debayer_img_pongb[j+1];
      //
//      wait();
      //
      
      wait();
			//bufdout.put(pix.r);
			bufdout.put(pixr_c);
			wait();
			//bufdout.put(pix.g);
			bufdout.put(pixg_c);
			wait();
			//bufdout.put(pix.b);
			bufdout.put(pixb_c);
 
			}
      
      
		/*
    	if (ping){
				//pix = debayer_img_ping[j];
			wait();
			//bufdout.put(pix.r);
			bufdout.put(debayer_img_pingr[j]);
			wait();
			//bufdout.put(pix.g);
			bufdout.put(debayer_img_pingg[j]);
			wait();
			//bufdout.put(pix.b);
			bufdout.put(debayer_img_pingb[j]);


			}
			else{
				//pix = debayer_img_pong[j];
			wait();
			//bufdout.put(pix.r);
			bufdout.put(debayer_img_pongr[j]);
			wait();
			//bufdout.put(pix.g);
			bufdout.put(debayer_img_pongg[j]);
			wait();
			//bufdout.put(pix.b);
			bufdout.put(debayer_img_pongb[j]);
			}
     */
		}
		ping = !ping;
	}
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif
