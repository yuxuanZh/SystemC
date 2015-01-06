#include "debayer.h"
#include "wami_params.h"
#include "wami_utils.h"
#include "wami_debayer.h"

void debayer::process_debayer()
{
  const OutputDataToken zero;
  //sink solve invalid data in reset
	data_out->put(zero);
  cout<<"dut reset "<<sc_time_stamp()<<endl;
	wait();

	while(true) {
		InputDataToken input;
		OutputDataToken output;
   // cout<<"time to initial dut"<<sc_time_stamp()<<endl;
    
		u16 input_data[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
		rgb_pixel output_data[WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD][WAMI_DEBAYER_IMG_NUM_COLS-2*PAD];

		wait();
    cout<<"DUT"<<sc_time_stamp()<<endl;
		input = data_in->get();
    //cout<<"DUT after get before put"<<sc_time_stamp()<<endl;
		for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS; i++)
			for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS; j++)
				input_data[i][j] = input.getElem(i * WAMI_DEBAYER_IMG_NUM_COLS + j);

		wami_debayer(output_data, input_data);

		for (int i = 0; i < WAMI_DEBAYER_IMG_NUM_ROWS-2*PAD; i++)
			for (int j = 0; j < WAMI_DEBAYER_IMG_NUM_COLS-2*PAD; j++)
				output.setElem(i * (WAMI_DEBAYER_IMG_NUM_COLS-2*PAD) + j,
					       output_data[i][j]);

		data_out->put(output);
    //cout<<"DUT after put before next"<<sc_time_stamp()<<endl;
	}
}
