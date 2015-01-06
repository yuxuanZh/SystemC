//reference /soc2014/examples/lecture13/MAC_FlexChannels
#include <systemc.h>
#include <ctos_flex_channels.h>

#ifndef __DEBAYER_H__
#define __DEBAYER_H__

#include "wami_params.h"

SC_MODULE(debayer) {

	// Declare I/O ports and initiators here
  sc_in<bool> clk;
  sc_in<bool> rst;
  get_initiator<u16> data_in;
  put_initiator<rgb_pixel> data_out;
  
	// Declare methods which are also SC_CTHREADS here
  void process_debayer();
  
	// Implement the module's constructor here
  SC_CTOR(debayer)
    : clk("clk")
    , rst("rst")
    , data_in("data_in")
    , data_out("data_out")
  {
    SC_CTHREAD(process_debayer, clk.pos());
    set_stack_size(0x1000000);
    reset_signal_is(rst, false);
    data_in.clk_rst(clk,rst);
    data_out.clk_rst(clk,rst);
  }

	// Declare member variables here
};

#endif