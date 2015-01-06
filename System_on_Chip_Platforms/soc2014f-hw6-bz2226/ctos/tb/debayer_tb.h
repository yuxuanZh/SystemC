#include <systemc.h>
#include <ctos_flex_channels.h>

#include <iostream>
#include <iomanip>
#include <ctime>
#include <ostream>

#ifndef __DEBAYERTB_H__
#define __DEBAYERTB_H__

#include "wami_params.h"
#include "wami_utils.h"

SC_MODULE(debayer_tb) {
  	sc_in<bool>        clk;         // clock
  	sc_in<bool>        rst;         // reset
  	sc_out<bool>       rst_dut;     // reset accelerator

	// DMA requests interface from memory to device
	sc_in<unsigned>   rd_index;     // array index (offset from starting address)
	sc_in<unsigned>   rd_length;    // burst size (in words)
	sc_in<bool>       rd_request;   // transaction request
	sc_out<bool>      rd_grant;     // transaction grant

	// DMA requests interface from device to memory
	sc_in<unsigned>   wr_index;   // array index (offset from starting address)
	sc_in<unsigned>   wr_length;  // burst size (in words)
	sc_in<bool>       wr_request; // transaction request
	sc_out<bool>      wr_grant;   // transaction grant

	// input data readen by load_input
	put_initiator<u16> bufdin;
	// output data written by store output
	get_initiator<u16> bufdout;

	sc_out<unsigned> conf_size;
	sc_out<bool>     conf_done;

	// computation complete. Written by store_output
	sc_in<bool> debayer_done;

  	void dmac(); //TB replaces the DMA controller

  	SC_CTOR(debayer_tb)
		: clk("clk")
		, rst("rst")
		, rst_dut("rst_dut")
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
		, num_bayer_pixels(IMG_NUM_ROWS*IMG_NUM_COLS)
		, num_debayer_pixels((IMG_NUM_ROWS-2*PAD)*(IMG_NUM_COLS-2*PAD))
		, bayer(new u16[num_bayer_pixels])
		, debayer(new rgb_pixel[num_debayer_pixels])
		, gold_debayer(new rgb_pixel[num_debayer_pixels])

        {
		mismatches = 0;

		SC_CTHREAD(dmac, clk.pos());
		reset_signal_is(rst, false);

		bufdin.clk_rst(clk,rst);
		bufdout.clk_rst(clk,rst);
	}


  	int getMismatches() {return mismatches;}

private:
  	int mismatches;    // number of mismatches
	const size_t num_bayer_pixels;
	const size_t num_debayer_pixels;
	u16* bayer;
	rgb_pixel* debayer;
	rgb_pixel* gold_debayer;
};

#endif
