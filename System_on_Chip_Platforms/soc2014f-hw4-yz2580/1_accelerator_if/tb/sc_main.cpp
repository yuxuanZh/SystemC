#include "systemc.h"
#include "debayer.h"
#include "debayer_tb.h"
#include <ctime>
#include <ostream>
#include <fstream>

// TODO: might need something here!!
#ifdef CTOS_MODEL
#include "debayer_ctos_wrapper.h"
#endif

ifstream input;             // input.txt
ifstream golden_output;     // golden_output.txt
int num_samples;            // number of input images

int sc_main(int, char **) {
	sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);
  
	input.open("../input/input.txt");
	golden_output.open("../input/golden_output.txt");
	int clk_in;     // T_clk
	int rst_in;     // t_rst
	input >> clk_in >> rst_in >> num_samples;
	cout << "Clock: " << clk_in << " ns; Reset hold: " << rst_in << " ns" << endl;

	sc_clock             clk("clk", clk_in, SC_NS);    // clock
	sc_signal<bool>      rst("rst");                   // reset
	sc_signal<bool>      rst_dut("rst_dut");           // reset

	sc_signal<unsigned>        rd_index("rd_index");
	sc_signal<unsigned>        rd_length("rd_length");
	sc_signal<bool>            rd_request("rd_request");
	sc_signal<bool>            rd_grant("rd_grant");
	sc_signal<unsigned>        wr_index("wr_index");
	sc_signal<unsigned>        wr_length("wr_length");
	sc_signal<bool>            wr_request("wr_request");
	sc_signal<bool>            wr_grant("wr_grant");
	put_get_channel<u16>       bufdin("bufdin");
	put_get_channel<rgb_pixel> bufdout("bufdout");
	sc_signal<unsigned>        conf_size("conf_size");
	sc_signal<bool>            conf_done("conf_done");
	sc_signal<bool>            debayer_done("debayer_done");

	// Declare TB and DUT modules. Make sure CtoS can instantiate the scheduled version
  #ifdef CTOS_MODELout
	debayer_ctos_wrapper dut("dut", CTOS_TARGET_SUFFIX(CTOS_MODEL));
#else
	// Original SystemC version
	debayer dut("dut");
#endif
  debayer_tb tb("tb");
	// Connect ports
  //DUT   
     
  dut.clk(clk);        // clock
	dut.rst(rst_dut);        // reset
	// DMA requests interface from memory to device
	dut.rd_index(rd_index);   // array index (offset from base address)
	dut.rd_length(rd_length);  // burst size
	dut.rd_request(rd_request); // transaction request
	dut.rd_grant(rd_grant);   // transaction grant
	// DMA requests interface from device to memory
	dut.wr_index(wr_index);   // array index (offset from base address)
	dut.wr_length(wr_length);  // burst size (in words)
	dut.wr_request(wr_request); // transaction request
	dut.wr_grant(wr_grant);   // transaction grant
	// input data read by load_input
	dut.bufdin(bufdin);
	// output data written by store output
  dut.bufdout(bufdout);
  //
	dut.conf_size(conf_size);
	dut.conf_done(conf_done);
	// computation complete
	dut.debayer_done(debayer_done);
  
  //TB
  tb.clk(clk);         // clock
  tb.rst(rst);         // reset
  tb.rst_dut(rst_dut);     // reset accelerator
	// DMA requests interface from memory to device
	tb.rd_index(rd_index);     // array index (offset from starting address)
	tb.rd_length(rd_length);    // burst size (in words)
	tb.rd_request(rd_request);   // transaction request
	tb.rd_grant(rd_grant);     // transaction grant
	// DMA requests interface from device to memory
	tb.wr_index(wr_index);   // array index (offset from starting address)
	tb.wr_length(wr_length);  // burst size (in words)
	tb.wr_request(wr_request); // transaction request
	tb.wr_grant(wr_grant);   // transaction grant
	// input data readen by load_input
	tb.bufdin(bufdin);
	// output data written by store output
  tb.bufdout(bufdout);
  //
	tb.conf_size(conf_size);
	tb.conf_done(conf_done);
	// computation complete. Written by store_output
	tb.debayer_done(debayer_done);
 
	// Reset and run simulation

  rst.write(false);
  sc_start(rst_in,SC_NS);
  rst.write(true);
  sc_start();
	/**
	 * The testbench calls sc_stop() after the last image has been
	 * processed. Please notice how the overall latency changes when
	 * you add FlexChannels interfaces to connect design submodules.
	 * When you will run scheduling with CtoS, please also notice
	 * how different micro-architectures translate into different
	 * latency/throughput.
	 */

	return 0;
}
