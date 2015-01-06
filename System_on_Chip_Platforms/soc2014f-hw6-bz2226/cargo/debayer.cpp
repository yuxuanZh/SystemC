// This is the solution to homework #4 except for bufdout type
// Please use this implementation for cargo

#include "debayer.hpp"

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


static int get_cb_row(int row)
{
	int cb_row = row;
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
u16 debayer::interp_G_at_RRR_or_G_at_BBB(u32 row, u32 col)
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
	u16 pos, neg, tmp;
	tmp = bayer_img[get_cb_row(row-1)][col];
	wait();
	pos = 2 * tmp;
	tmp = bayer_img[get_cb_row(row)][col-1];
	wait();
	pos += 2 * tmp;
	tmp = bayer_img[get_cb_row(row)][col];
	wait();
	pos += 4 * tmp;
	tmp = bayer_img[get_cb_row(row)][col+1];
	wait();
	pos += 2 * tmp;
	tmp = bayer_img[get_cb_row(row+1)][col];
	wait();
	pos +=	2 * tmp;
	tmp = bayer_img[get_cb_row(row)][col+2];
	wait();
	neg = tmp;
	tmp = bayer_img[get_cb_row(row-2)][col];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row)][col-2];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+2)][col];
	wait();
	neg += tmp;

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
	u16 pos, neg, tmp1, tmp2, tmp;
	tmp1 = bayer_img[get_cb_row(row+2)][col];
	wait();
	tmp2 = bayer_img[get_cb_row(row-2)][col];
	wait();
	tmp = ((tmp1 + tmp2) >> 1);
	pos = tmp;
	tmp = bayer_img[get_cb_row(row)][col-1];
	wait();
	pos += 4 * tmp;
	tmp = bayer_img[get_cb_row(row)][col];
	wait();
	pos += 5 * tmp;
	tmp = bayer_img[get_cb_row(row)][col+1];
	wait();
	pos += 4 * tmp;
	tmp = bayer_img[get_cb_row(row-1)][col-1];
	wait();
	neg = tmp;
	tmp = bayer_img[get_cb_row(row-1)][col+1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row)][col-2];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row)][col+2];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+1)][col-1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+1)][col+1];
	wait();
	neg += tmp;

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
	u16 pos, neg, tmp1, tmp2, tmp;
	tmp1 = bayer_img[get_cb_row(row)][col-2];
	wait();
	tmp2 = bayer_img[get_cb_row(row)][col+2];
	wait();
	tmp = ((tmp1 + tmp2) >> 1);
	pos = tmp;
	tmp = bayer_img[get_cb_row(row-1)][col];
	wait();
	pos += 4 * tmp;
	tmp = bayer_img[get_cb_row(row)][col];
	wait();
	pos += 5 * tmp;
	tmp = bayer_img[get_cb_row(row+1)][col];
	wait();
	pos += 4 * tmp;
	tmp = bayer_img[get_cb_row(row-2)][col];
	wait();
	neg = tmp;
	tmp = bayer_img[get_cb_row(row-1)][col-1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row-1)][col+1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+1)][col-1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+1)][col+1];
	wait();
	neg += tmp;
	tmp = bayer_img[get_cb_row(row+2)][col];
	wait();
	neg += tmp;

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
	u16 pos, neg, tmp;
	tmp = bayer_img[get_cb_row(row-1)][col-1];
	wait();
	pos = 2 * tmp;
	tmp = bayer_img[get_cb_row(row-1)][col+1];
	wait();
	pos += 2 * tmp;
	tmp = bayer_img[get_cb_row(row)][col];
	wait();
	pos += 6 * tmp;
	tmp = bayer_img[get_cb_row(row+1)][col-1];
	wait();
	pos += 2 * tmp;
	tmp = bayer_img[get_cb_row(row+1)][col+1];
	wait();
	pos += 2 * tmp;
	tmp = bayer_img[get_cb_row(row-2)][col];
	wait();
	neg = 3 * tmp;
	tmp = bayer_img[get_cb_row(row)][col-2];
	wait();
	neg += 3 * tmp;
	tmp = bayer_img[get_cb_row(row)][col+2];
	wait();
	neg += 3 * tmp;
	tmp = bayer_img[get_cb_row(row+2)][col];
	wait();
	neg += 3 * tmp;

	return compute_and_clamp_pixel_fractional_neg(pos, neg);
}


void debayer::wami_debayer(int cols, int img_row,
			   int cb_row, bool ping)
{
	u32 col;

	/*
	 * Demosaic the following Bayer pattern:
	 * R G ...
	 * G B ...
	 * ... ...
	 */

	/* Copy red pixels through directly */
	if (!(img_row % 2)) //even rows
	{
	FOR_COPY_RED:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = bayer_img[cb_row][col];
			// Need 1 cycle to ready from scratchpad memory
			wait();
			// Choose output buffer
			if (ping)
				debayer_img_ping[col-PAD].r = pix;
			else
				debayer_img_pong[col-PAD].r = pix;
		}
	}

	/* Copy top-right green pixels through directly */
	if (!(img_row % 2))
	{
	FOR_COPY_GREEN_TR:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = bayer_img[cb_row][col];
			wait();
			if (ping)
				debayer_img_ping[col-PAD].g = pix;
			else
				debayer_img_pong[col-PAD].g = pix;
		}
	}

	/* Copy bottom-left green pixels through directly */
	if (img_row % 2) //odd rows
	{
	FOR_COPY_GREEN_BL:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = bayer_img[cb_row][col];
			wait();
			if (ping)
				debayer_img_ping[col-PAD].g = pix;
			else
				debayer_img_pong[col-PAD].g = pix;
		}
	}

	/* Copy blue pixels through directly */
	if (img_row % 2)
	{
	FOR_COPY_BLUE:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = bayer_img[cb_row][col];
			wait();
			if (ping)
				debayer_img_ping[col-PAD].b = pix;
			else
				debayer_img_pong[col-PAD].b = pix;
		}
	}

	/* Interpolate green pixels at red pixels */
	if (!(img_row % 2))
	{
	FOR_INTERP_GR:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_G_at_RRR_or_G_at_BBB(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].g = pix;
			else
				debayer_img_pong[col-PAD].g = pix;
		}
	}

	/* Interpolate green pixels at blue pixels */
	if (img_row % 2)
	{
	FOR_INTERP_GB:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_G_at_RRR_or_G_at_BBB(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].g = pix;
			else
				debayer_img_pong[col-PAD].g = pix;
		}
	}

	/* Interpolate red pixels at green pixels, red row, blue column */
	if (!(img_row % 2))
	{
	FOR_INTERP_RG_RR_BC:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].r = pix;
			else
				debayer_img_pong[col-PAD].r = pix;
		}
	}

	/* Interpolate blue pixels at green pixels, blue row, red column */
	if (img_row % 2)
	{
	FOR_INTERP_BG_BR_RC:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_GRB_or_B_at_GBR(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].b = pix;
			else
				debayer_img_pong[col-PAD].b = pix;
		}
	}

	/* Interpolate red pixels at green pixels, blue row, red column */
	if (img_row % 2)
	{
	FOR_INTERP_RG_BR_RC:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_GBR_or_B_at_GRB(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].r = pix;
			else
				debayer_img_pong[col-PAD].r = pix;
		}
	}

	/* Interpolate blue pixels at green pixels, red row, blue column */
	if (!(img_row % 2))
	{
	FOR_INTERP_BG_RR_BC:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_GBR_or_B_at_GRB(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].b = pix;
			else
				debayer_img_pong[col-PAD].b = pix;
		}
	}

	/* Interpolate red pixels at blue pixels, blue row, blue column */
	if (img_row % 2)
	{
	FOR_INTERP_RB_BR_BC:
		for (col = PAD+1; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_BBB_or_B_at_RRR(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].r = pix;
			else
				debayer_img_pong[col-PAD].r = pix;
		}
	}

	/* Interpolate blue pixels at red pixels, red row, red column */
	if (!(img_row % 2))
	{
	FOR_INTERP_BR_RR_RC:
		for (col = PAD; col < WAMI_DEBAYER_IMG_NUM_COLS-PAD; col += 2)
		{
			if (col >= cols-PAD)
				break;
			u16 pix = interp_R_at_BBB_or_B_at_RRR(cb_row, col);
			if (ping)
				debayer_img_ping[col-PAD].b = pix;
			else
				debayer_img_pong[col-PAD].b = pix;
		}
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
//cout << "SIZE IN DEBAYER ACC " << size.read() <<endl;
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

	const int rows = size.read();

	int index = 0;
	// Use flag to fill in the buffer with the first five rows
	int length = 5 * rows;
	bool cold_start = true;
	int cb_row = 0;

LOAD_INPUT_WHILE:
	while(true) {
    // cout << "row in load " << rows <<endl; 
		if (index == rows * rows)
			// Input complete; wait for reset
			do { wait(); }	while (true);

		// Send DMA request
		if (!cold_start) length = rows;
		rd_index.write(index);
		rd_length.write(length);
		// This implementation processes one row at a time
		index += length;
		// 4-phase handshake
		rd_request.write(true);
		do { wait(); } while(!rd_grant.read());
		rd_request.write(false);

		if (cold_start) {
		LOAD_INIT_LOOP:
			for (int k = 0; k < 5; k++)
			LOAD_INIT_INNER_LOOP:
				for (int j = 0; j < IMG_NUM_COLS; j++) {
					if (j == rows)
						break;
					u16 pix = bufdin.get();
					bayer_img[k][j] = pix;
					wait();
				}
			cb_row += 5;
			cold_start = false;
		}
		else {
		LOAD_SINGLE_LOOP:
			for (int j = 0; j < IMG_NUM_COLS; j++) {
				if (j == rows)
					break;
				u16 pix = bufdin.get();
				bayer_img[cb_row][j] = pix;
				wait();
			}
			cb_row++;
     // cout << "CB_ROW " << cb_row << endl; 
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

	// Wait for configuratin
	do { wait(); } while (!init_done.read());

	const int rows = size.read();
	int cb_row = PAD;
	int img_row = PAD;
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
		wami_debayer(rows, img_row, cb_row, ping);

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

	const int rows = size.read() - (2 * PAD);
	int index = 0;
	int length = rows;
	bool ping = true;


  int cnt = 0; 
  
STORE_OUTPUT_WHILE:
	while(true) {

		if (index == rows * rows) {
			// DEBAYER Done (need a reset)
			debayer_done.write(true);
     // cout<<"HERE"<<endl;
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

		// 4-phase handshake
		wr_request.write(true);
		do { wait(); } while(!wr_grant.read());
		wr_request.write(false);

	STORE_SINGLE_LOOP:
		for (int j = 0; j < IMG_NUM_COLS - 2 * PAD; j++) {
			if (j == rows)
				break;
			rgb_pixel pix;
      
      
			if (ping)
				pix = debayer_img_ping[j];
			else
				pix = debayer_img_pong[j];
        
         //cout << "pix_r[" << cnt << "]" << pix.r << endl;
         cnt++;
        
			wait();
			bufdout.put(pix.r);
			wait();
			bufdout.put(pix.g);
			wait();
			bufdout.put(pix.b);
		}
		ping = !ping;
	}
}

#ifdef __CTOS__
SC_MODULE_EXPORT(debayer)
#endif
