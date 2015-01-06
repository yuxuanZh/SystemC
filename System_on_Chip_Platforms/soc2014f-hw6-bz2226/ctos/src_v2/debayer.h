#include <systemc.h>
#include <ctos_flex_channels.h>

#ifndef __DEBAYER_H__
#define __DEBAYER_H__

#include "wami_params.h"

#define CB_SIZE 6

SC_MODULE(debayer) {
	// Declare I/O ports and initiators here
	sc_in<bool>       clk;        // clock
	sc_in<bool>       rst;        // reset

	// DMA requests interface from memory to device
	sc_out<unsigned> rd_index;   // array index (offset from base address)
	sc_out<unsigned> rd_length;  // burst size
	sc_out<bool>     rd_request; // transaction request
	sc_in<bool>      rd_grant;   // transaction grant

	// DMA requests interface from device to memory
	sc_out<unsigned> wr_index;   // array index (offset from base address)
	sc_out<unsigned> wr_length;  // burst size
	sc_out<bool>     wr_request; // transaction request
	sc_in<bool>      wr_grant;   // transaction grant

	// input data read by load_input
	get_initiator<u16> bufdin;
	// output data written by store output
	put_initiator<u16> bufdout;

	sc_in<unsigned> conf_size;
	sc_in<bool>     conf_done;

	// computation complete
	sc_out<bool> debayer_done;

	// Declare methods which are also SC_CTHREADS here
	void config_debayer();
	void load_input();
	void store_output();
  	void process_debayer();    // process for debayer

	// Implement the module's constructor here
  	SC_CTOR(debayer)
		: clk("clk")
		, rst("rst")
		, rd_index("rd_index")
		, rd_length("rd_length")
		, rd_request("rd_request")
		, rd_grant("rd_grant")
		, wr_index("wr_index")
		, wr_length("wr_length")
		, wr_request("wr_request")
		, wr_grant("wr_grant")
		, bufdin("bufdin")
		, bufdout("bufdout")
		, conf_size("conf_size")
		, conf_done("conf_done")
		, debayer_done("debayer_done")
	{
		SC_CTHREAD(config_debayer, clk.pos());
		reset_signal_is(rst, false);
		SC_CTHREAD(load_input, clk.pos());
		reset_signal_is(rst, false);
		SC_CTHREAD(store_output, clk.pos());
		reset_signal_is(rst, false);

		SC_CTHREAD(process_debayer, clk.pos());
		reset_signal_is(rst, false);

		bufdin.clk_rst(clk,rst);
		bufdout.clk_rst(clk,rst);
  	}

	// Declare here class methods that are not processes.
	// These functions can access member variables



	/**
	 * @cols: size of the image in terms of number of columns
	 * @img_row: current image row. Need to know wether it's odd or even
	 * @cb_row: footprint central row in the circular buffer
	 * @ping: flag to determne which will be the output buffer
	 */
	void wami_debayer(u16 cols, u16 img_row,
			char cb_row,bool ping);

	// Circular buffer for input rows
	u16 bayer_img[CB_SIZE][IMG_NUM_COLS];
	// Duplicate output buffer to make CtoS instantiate two memories
	//rgb_pixel debayer_img_ping[IMG_NUM_COLS-2*PAD];
	//rgb_pixel debayer_img_pong[IMG_NUM_COLS-2*PAD];
	u16 debayer_img_pingr[IMG_NUM_COLS-2*PAD];
  u16 debayer_img_pingg[IMG_NUM_COLS-2*PAD];
  u16 debayer_img_pingb[IMG_NUM_COLS-2*PAD];
 	u16 debayer_img_pongr[IMG_NUM_COLS-2*PAD];
  u16 debayer_img_pongg[IMG_NUM_COLS-2*PAD];
  u16 debayer_img_pongb[IMG_NUM_COLS-2*PAD];

	//Written by config_debayer
	sc_signal<unsigned> size;
	sc_signal<bool>     init_done;

	//Written by load_input
	sc_signal<bool>     input_done;

	//Written by process_debayer
	sc_signal<bool>     process_start;
	sc_signal<bool>     process_done;

	//Written by store_output
	sc_signal<bool>     output_start;
	//sc_signal<bool>     output_busy;
 
  //
  //sc_signal<u16> img_row_s;

};

#endif
