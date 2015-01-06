#include "systemc.h"
#include "debayer.h"
#include "debayer_tb.h"
#include "mydata.h"
#include <ctime>
#include <ostream>
#include "wami_params.h"



int sc_main(int, char**) {
  char *input_txt = "../input/input.txt";
  int tclk,trst,tsim;
  ifstream input(input_txt);
  input>>tclk>>trst>>tsim;

  sc_clock  clk("clk",tclk,SC_NS);
  sc_signal<bool>rst("rst");
  sc_signal<InputImage>inimg;
  sc_signal<OutputImage>outimg;

  debayer dut("dut");
  debayer_tb tb("tb");

  dut.clk(clk);
  dut.rst(rst);
  dut.outimg(outimg);
  dut.inimg(inimg);  

  tb.clk(clk);
  tb.rst(rst);
  tb.outimg(outimg);
  tb.inimg(inimg);

  rst.write(false);
  sc_start(trst,SC_NS);

  rst.write(true);
  sc_start(tsim,SC_NS);

  sc_stop();
 
  return 0;
}
