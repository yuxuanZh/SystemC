#include "systemc.h"
#include "tlm.h"
#include "debayer.h"
#include "debayer_tb.h"
#include <ctime>
#include <ostream>
#include <fstream>

#include "wami_params.h"
#include "wami_utils.h"
#include "wami_debayer.h"
#include "mydata.h"

using namespace tlm;

ifstream input;             // input.txt
ifstream golden_output;     // golden_output.txt
ofstream result;            // result.txt

int sc_main(int, char **) {
	input.open("../input/input.txt");
	golden_output.open("../input/golden_output.txt");
	result.open("../output/result.txt");
	int clk_in;     // T_clk
	int rst_in;     // t_rst
	int sim_in;     // T_sim
	input >> clk_in >> rst_in >> sim_in;
	cout << "Clock: " << clk_in << "ns; Reset hold: " << rst_in;
	cout << "ns; Simulation time: " << sim_in << "ns" << endl;

	sc_clock             clk("clk", clk_in, SC_NS);    // clock
	sc_signal<bool>      rst("rst");                   // reset
	tlm_fifo<InputDataToken> data_in;//tlm fifo
	tlm_fifo<OutputDataToken> data_out;//tlm_fifo

	debayer dut("dut");   // DUT
	debayer_tb tb("tb");  // testbench

	dut.clk(clk);     // connect DUT
	dut.rst(rst);
	dut.data_in(data_in);
	dut.data_out(data_out);

	tb.clk(clk);      // connect testbench
	tb.rst(rst);
	tb.data_in(data_in);
	tb.data_out(data_out);


	sc_trace_file *fp;                                  // Declare FilePointer fp
	fp = sc_create_vcd_trace_file("debayer");             // Open the VCD file: dft.vcd
	((vcd_trace_file *) fp)->sc_set_vcd_time_unit(-9);  // Set default time unit to ns
	/*sc_trace(fp, clk, "clk");                           // Add signals to trace file
	sc_trace(fp, rst, "rst");
	sc_trace(fp, data_in, "data_in");
	sc_trace(fp, data_out, "data_out");
  */
	rst.write(false);
	sc_start(rst_in,SC_NS);               // reset for a while
	rst.write(true);

	sc_start(sim_in,SC_NS);               // simulate

	sc_stop();

	int errors = tb.getMissmatches();     // show the testing summary
	if (errors > 0)
		cout << "Simulation with " << errors << " miss-matchess." << endl;
	else
		cout << "Simulation successful @ " << sc_time_stamp() << endl;

	sc_close_vcd_trace_file(fp);          // close all the files
	input.close();
	golden_output.close();
	result.close();

	return 0;
}
