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
  	b_put_initiator<u16>  data_in;     // sample data
  	b_get_initiator<rgb_pixel>   data_out;    // ordered output
   
    void source();
    void sink();
    int getMissmatches() {return missmatches;}

  SC_CTOR(debayer_tb)
    : clk("clk")
    , rst("rst")
    , data_in("data_in")
    , data_out("data_out")
  {
    SC_THREAD(source);
    set_stack_size(0x1000000);
    sensitive << clk.pos();
    SC_THREAD(sink);
    set_stack_size(0x1000000);
    sensitive << clk.pos();
    data_in.clk_rst(clk,rst);
    data_out.clk_rst(clk,rst);  
  }
  
  private:
  	int missmatches;    // number of missmatches
};

#endif
