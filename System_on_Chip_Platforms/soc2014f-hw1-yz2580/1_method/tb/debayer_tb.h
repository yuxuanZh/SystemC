#ifndef __DEBAYERTB_H__
#define __DEBAYERTB_H__

#include <ctime>
#include <ostream>

#include "systemc.h"
#include "wami_params.h"
#include "mydata.h"


SC_MODULE(debayer_tb){
 sc_in<bool>clk;
 sc_in<bool>rst;
 sc_in<OutputImage>outimg;
 sc_out<InputImage>inimg;

 void source();
 void sink();
 
 SC_CTOR(debayer_tb){
  SC_THREAD(source);
  SC_THREAD(sink);
}
private:
 sc_signal<OutputImage>goldimg;
};

#endif
