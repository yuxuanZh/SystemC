#ifndef __DEBAYER_H__
#define __DEBAYER_H__

#include "systemc.h"
#include "wami_debayer.h"
#include "wami_params.h"
#include "mydata.h"

SC_MODULE(debayer){
 sc_in<bool>clk;
 sc_in<bool>rst;
 sc_out<OutputImage>outimg;
 sc_in<InputImage>inimg;

 void beh();

 SC_CTOR(debayer){
  SC_METHOD(beh);
  sensitive<<clk.pos()<<rst;
}
// private:
 // sc_signal<>
};
#endif