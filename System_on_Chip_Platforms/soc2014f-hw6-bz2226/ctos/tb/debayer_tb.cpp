#include "debayer_tb.h"

extern ifstream input;
extern ifstream golden_output;
extern int num_samples;

void debayer_tb::dmac()
{
	// RESET
	rst_dut.write(false);

	bufdin.reset_put();
	bufdout.reset_get();

	rd_grant.write(false);
	wr_grant.write(false);

	conf_size.write(0);
	conf_done.write(false);

	int count = 0;
	unsigned rows, cols;

	wait();

	while(count < num_samples) {

		if (!rst_dut.read()) {
			//De-activate DUT reset
			rst_dut.write(true);
			wait();
			//Configure TB for input and validation
			input >> rows;
			cols = rows;
			std::string test_filename;
			input >> test_filename;
			read_image_file(bayer, sizeof(u16),
				test_filename.c_str(), ".",
				sizeof(u16) * rows * rows);
			//Configure DUT
			cout << "Configuration: " << rows << endl;
			conf_size.write(rows);
			wait();
			conf_done.write(true);
			wait();
		}

		do { wait(); }
		while (!rd_request.read() && !wr_request.read() && !debayer_done.read());

		if (debayer_done.read()) {
			cout << "TB: DEBAYER done @ " << sc_time_stamp() << endl;
			// Reset DUT
			rst_dut.write(false);
			wait();

			// Write output file and validate
			std::string exp_filename;
			golden_output >> exp_filename;
			std::stringstream res_filename;
			res_filename << "../output/" << std::setw(4) << std::setfill('0') << count << ".bin";
			write_image_file((void *) debayer, sizeof(u16),
					 res_filename.str().c_str(), ".",
					 rows-2*PAD, cols-2*PAD, 3);
			read_image_file((void *) gold_debayer, sizeof(rgb_pixel),
					exp_filename.c_str(), ".",
					sizeof(rgb_pixel) * (rows-2*PAD)*(cols-2*PAD));
			for (int i = 0; i < (rows-2*PAD)*(rows-2*PAD); i++) {
				if (gold_debayer[i].r != debayer[i].r ||
				    gold_debayer[i].g != debayer[i].g ||
				    gold_debayer[i].b != debayer[i].b) {
					mismatches++;
					cout << "debayer[" << i << "] doesn't match" << endl;
					cout << debayer[i] << " result !===! expected " << gold_debayer[i] << endl;
					break;
				}
			}
			cout << endl;
			// Process next image if any
			count++;
			continue;
		}

		if (rd_request.read()) {
			//READ REQUEST
			unsigned index = rd_index.read();
			unsigned length = rd_length.read();
			// 4-phases handshake
			rd_grant.write(true);
			do { wait(); }
			while (rd_request.read());
			rd_grant.write(false);
			wait();

			// cout << "TB: READ BURST; len = " << length
			//      << "; index = " << index << endl;

			// DMA from memory to device
			for(unsigned i = index; i < index+length; i++) {
				if (i == rows * rows) {
					cout << "TB: Error: EOI" << endl;
					break;
				}
				bufdin.put(bayer[i]);
				wait();
			}
			// cout << "DMAC: input done @" << sc_time_stamp() << endl;
		} else {
			//WRITE REQUEST
			unsigned index = wr_index.read();
			unsigned length = wr_length.read();
			// 4-phases handshake
			wr_grant.write(true);
			do { wait(); }
			while (wr_request.read());
			wr_grant.write(false);
			wait();

			// cout << "TB: WRITE BURST; len = " << length
			//      << "; index = " << index << endl;

			// DMA from device to memory
			for(unsigned i = index; i < index+length; i++) {
				if (i == (rows-2*PAD)*(rows-2*PAD)) {
					cout << "TB: Error: EOO" << endl;
					break;
				}

				wait();
				debayer[i].r = bufdout.get();
				wait();
				debayer[i].g = bufdout.get();
				wait();
				debayer[i].b = bufdout.get();
			}

			// cout << "DMAC: output done @" << sc_time_stamp() << endl;
		}
	}

	// Stop simulation
	sc_stop();
	int errors = getMismatches();     // show the testing summary
	if (errors > 0)
		cout << "Simulation with " << errors << " mismatches." << endl;
	else
		cout << "Simulation successful! @ " << sc_time_stamp() << endl;
	input.close();
	golden_output.close();
}
