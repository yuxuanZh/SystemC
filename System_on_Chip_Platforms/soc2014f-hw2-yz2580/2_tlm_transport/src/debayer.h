#ifndef __DEBAYER_H__
#define __DEBAYER_H__

#include "systemc.h"
#include "mydata.h"
#include "doDebayer.h"

SC_MODULE(debayer) {
  sc_in<bool> clk;
  sc_in<bool> rst;
  sc_port<tlm_blocking_put_if<OutputDataToken> > data_out;//tlm blocking interface
  sc_port<tlm_blocking_get_if<InputDataToken> > data_in;
  sc_port<tlm_transport_if<InputDataToken,OutputDataToken> > doDebayer;//tlm transport interface

  void process_debayer();    // process for debayer

  	SC_CTOR(debayer) {
		SC_CTHREAD(process_debayer, clk.pos());
		set_stack_size(0x1000000);
		reset_signal_is(rst, false);
  	}

};
#endif
