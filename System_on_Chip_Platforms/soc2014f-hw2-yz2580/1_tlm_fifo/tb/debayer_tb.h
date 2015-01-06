#ifndef __DEBAYERTB_H__
#define __DEBAYERTB_H__

#include <ctime>
#include <ostream>
#include "systemc.h"
#include "tlm.h"
#include "wami_params.h"
#include "mydata.h"

using namespace tlm;

SC_MODULE(debayer_tb){
 sc_in<bool>clk;
 sc_in<bool>rst;
 sc_port<tlm_blocking_get_if<OutputDataToken> > data_out; //sc_port blocking interface
 sc_port<tlm_blocking_put_if<InputDataToken> > data_in;

    void source();      // to generate test pattern
  	void sink();        // to validate result
  	int getMissmatches() {return missmatches;}

  	SC_CTOR(debayer_tb) {
		missmatches = 0;
		SC_THREAD(source);
		set_stack_size(0x1000000);
		SC_THREAD(sink);
		set_stack_size(0x1000000);
  	}

private:
  	int missmatches;    // number of missmatches
};

#endif