#include "debayer_tb.h"
#define OUTPUT_TOKEN_SIZE (WAMI_DEBAYER_IMG_NUM_ROWS - 2 * WAMI_DEBAYER_PAD) * (WAMI_DEBAYER_IMG_NUM_COLS - 2 * WAMI_DEBAYER_PAD)

extern ifstream input;
extern ifstream golden_output;
extern int num_samples;

void debayer_tb::source()
{
 RESET:
  data_in.reset_put();
  input >> std::dec >> num_samples;
  cout<< "Source reset " << sc_time_stamp() <<endl;
  int count = 0;
  do {
    wait();
  } while(!rst.read());
  
  while(true) {
  
    u16 bayer[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
		//rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];
  
		std::string test_filename;
		//int time_stamp;

		u16 test_data[WAMI_DEBAYER_IMG_NUM_ROWS * WAMI_DEBAYER_IMG_NUM_COLS];

		//input >> std::dec >> time_stamp;
		input >> test_filename;
		read_image_file((void *) test_data, sizeof(u16), test_filename.c_str(),
			 ".", sizeof(u16) * WAMI_DEBAYER_IMG_NUM_ROWS * WAMI_DEBAYER_IMG_NUM_COLS);

		for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++)
			 bayer[i][j] = test_data[i*WAMI_DEBAYER_IMG_NUM_ROWS + j];
   
    cout<< "Source put before " << sc_time_stamp() <<endl;
   
    for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++){
       data_in.put(bayer[i][j]);
       //wait();
      }
    cout<< "Source put after " << sc_time_stamp() <<endl;
		cout << std::dec << "@ " << sc_time_stamp();
		cout << " test_filename: " << test_filename << endl << endl;
    count++;
    cout << "Source count: " << count<< endl;
    if (count == num_samples) {cout<< "Souce break" <<endl; break;}
    wait();
	}
}

void debayer_tb::sink()
{ 
  missmatches = 0;
  int count = 0;
  data_out.reset_get();
  cout<< "Sink reset " << sc_time_stamp() <<endl;
  
  do {
    wait();
  } while(!rst.read());

	while(true) {
    
		rgb_pixel res_data[OUTPUT_TOKEN_SIZE];
		rgb_pixel exp_data[OUTPUT_TOKEN_SIZE];

		//int time_stamp;
		std::string exp_filename;
		std::stringstream res_filename;
		res_filename << "../output/" << std::setw(4) << std::setfill('0') << count << ".bin";

		//golden_output >> std::dec >> time_stamp;
		golden_output >> exp_filename;
    
    cout<< "Sink get before " << sc_time_stamp() <<endl;
    
    for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD; i++)
    for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD; j++){
		 res_data[i*(WAMI_DEBAYER_IMG_NUM_ROWS - 2 * WAMI_DEBAYER_PAD) + j] = data_out.get();
      //wait();
     } 
    cout<< "Sink get after " << sc_time_stamp() <<endl;
		//cout << std::dec << "check time stamp ";
		//cout << time_stamp << ";" << endl;
		cout << "@ " << sc_time_stamp();
		cout << " exp_filename: " << exp_filename << endl;
    cout << " res_filename: " << res_filename.str() << endl;
		//result << std::dec << sc_time_stamp().value() / 1000;
		//result << " " << res_filename.str().c_str() << endl;

		write_image_file((void *) res_data, sizeof(u16),
			 res_filename.str().c_str(), ".", WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD,
			 WAMI_DEBAYER_IMG_NUM_COLS-2*PAD, 3);

		read_image_file((void *) exp_data, sizeof(rgb_pixel),
			 exp_filename.c_str(), ".", sizeof(rgb_pixel) * OUTPUT_TOKEN_SIZE);

		for (int i = 0; i < OUTPUT_TOKEN_SIZE; i++) {
			if (exp_data[i].r != res_data[i].r ||
					exp_data[i].g != res_data[i].g ||
					exp_data[i].b != res_data[i].b) {
				missmatches++;
				cout << std::dec << "@ " << sc_time_stamp();
				cout << " : EXPECT: " << exp_filename << endl;
				break;
			}
		}
		count++;
   	cout << "Sink count: " << count<< endl;
    if(count == num_samples) {cout<< "Sink break" <<endl; break;}
    wait();
	}
    sc_stop();
    
   	if (missmatches > 0)
		cout << "Simulation with " << missmatches << " miss-matchess." << endl;
	else
		cout << "Simulation successful @ " << sc_time_stamp() << endl;

	//sc_close_vcd_trace_file(fp);          // close all the files
	input.close();
	golden_output.close();
}
