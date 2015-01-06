#include "systemc.h"
#include "debayer.h"
#include "debayer_tb.h"
#include <ctime>
#include <ostream>
#include <fstream>


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

	input >> clk_in >> rst_in;
	cout << "Clock: " << clk_in << "ns; Reset hold: " << rst_in <<endl;

	sc_clock             clk("clk", clk_in, SC_NS);    // clock
	sc_signal<bool>      rst("rst");                   // reset
	put_get_channel <u16> data_in("data_in");                      // sample data
	put_get_channel <rgb_pixel> data_out("data_out");                     // ordered result
 
#ifdef CTOS_MODELout
	debayer_ctos_wrapper dut("dut", CTOS_TARGET_SUFFIX(CTOS_MODEL));
#else
	// Original SystemC version
	debayer dut("dut");
#endif
	debayer_tb tb("tb");  // testbench

  dut.clk(clk);     // connect DUT
	dut.rst(rst);
	dut.data_in(data_in);
	dut.data_out(data_out);
 
	tb.clk(clk);      // connect testbench
	tb.rst(rst);
	tb.data_in(data_in);
	tb.data_out(data_out);
/*
	sc_trace_file *fp;                                  // Declare FilePointer fp
	fp = sc_create_vcd_trace_file("debayer");             // Open the VCD file: dft.vcd
	((vcd_trace_file *) fp)->sc_set_vcd_time_unit(-9);  // Set default time unit to ns
	sc_trace(fp, clk, "clk");                           // Add signals to trace file
	sc_trace(fp, rst, "rst");
	sc_trace(fp, data_in, "data_in");
	sc_trace(fp, data_out, "data_out");
 */
  
	rst.write(false);
	sc_start(rst_in,SC_NS);              // reset for a while
	rst.write(true);
  
	sc_start();               // simulate

	/*
  int errors = tb.getMissmatches();     // show the testing summary
	if (errors > 0)
		cout << "Simulation with " << errors << " miss-matchess." << endl;
	else
		cout << "Simulation successful @ " << sc_time_stamp() << endl;

	//sc_close_vcd_trace_file(fp);          // close all the files
	input.close();
	golden_output.close();
*/
	return 0;

}


