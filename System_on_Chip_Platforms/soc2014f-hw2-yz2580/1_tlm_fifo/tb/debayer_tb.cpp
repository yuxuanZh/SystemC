#include <iostream>
#include <iomanip>

#include "debayer_tb.h"

#include "wami_params.h"
#include "wami_utils.h"
#include "wami_debayer.h"


extern ifstream input;
extern ifstream golden_output;
extern ofstream result;

void debayer_tb::source()
{
	wait(rst.posedge_event());

	while(true) {
		InputDataToken test;
		wait(clk.posedge_event());

		std::string test_filename;
		int time_stamp;

		u16 test_data[INPUT_TOKEN_SIZE];

		input >> std::dec >> time_stamp;
		input >> test_filename;

		read_image_file((void *) test_data, sizeof(u16), test_filename.c_str(),
			 ".", sizeof(u16) * INPUT_TOKEN_SIZE);

		for (int i = 0; i < INPUT_TOKEN_SIZE; i++) {
			test.setElem(i, test_data[i]);
		}

		data_in->put(test);
		cout << std::dec << "@ " << sc_time_stamp();
		cout << " : INPUT: " << test_filename << endl << endl;
	}
}

void debayer_tb::sink()
{
	int count = 0;
  const OutputDataToken zero;
  OutputDataToken tmp;
  
  //get invalid output from dut in reset
  while(!rst.read())
   {tmp = data_out->get();
    cout<<"reset"<<sc_time_stamp()<<endl;
    wait(clk.posedge_event());}
   
	while(true) {
		OutputDataToken res;
		OutputDataToken expected;

		rgb_pixel res_data[OUTPUT_TOKEN_SIZE];
		rgb_pixel exp_data[OUTPUT_TOKEN_SIZE];

		wait(clk.posedge_event());

		int time_stamp;
		std::string exp_filename;
		std::stringstream res_filename;
		res_filename << "../output/" << std::setw(4) << std::setfill('0') << count << ".bin";

		golden_output >> std::dec >> time_stamp;
		golden_output >> exp_filename;
   	
       res = data_out->get();
 
		cout << std::dec << "check time stamp ";
		cout << time_stamp << ";" << endl;
		cout << "@ " << sc_time_stamp();
		cout << " : OUTPUT: " << exp_filename << endl;

		result << std::dec << sc_time_stamp().value() / 1000;
		result << " " << res_filename.str().c_str() << endl;


		for (int i = 0; i < OUTPUT_TOKEN_SIZE; i++) {
			res_data[i] = res.getElem(i);
		}

		write_image_file((void *) res_data, sizeof(u16),
			 res_filename.str().c_str(), ".", WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD,
			 WAMI_DEBAYER_IMG_NUM_COLS-2*PAD, 3);

		read_image_file((void *) exp_data, sizeof(rgb_pixel),
			 exp_filename.c_str(), ".", sizeof(rgb_pixel) * OUTPUT_TOKEN_SIZE);

		for (int i = 0; i < OUTPUT_TOKEN_SIZE; i++) {
			expected.setElem(i, exp_data[i]);
		}


		for (int i = 0; i < OUTPUT_TOKEN_SIZE; i++) {
			if (expected.getElem(i).r != res.getElem(i).r ||
					expected.getElem(i).g != res.getElem(i).g ||
					expected.getElem(i).b != res.getElem(i).b) {
				missmatches++;
        cout<<"wrong"<<endl;
				cout << std::dec << "@ " << sc_time_stamp();
				cout << " : EXPECT: " << exp_filename << endl;
				break;
			}
		}
		cout << endl;
		count++;
	}
  
}
