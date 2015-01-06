#include "debayer_tb.h"

extern ifstream input;
extern ifstream golden_output;
extern int num_samples;


void debayer_tb::dmac()
{
	/**
	 * Implement the testbench as a single process that replaces a DMA
	 * controller. DMA allows to transfer data from the main memory
	 * (usually DRAM) to the local memory of a device (a scratch pad).
	 * Debayer must use its interface's signals to tell the DMAC which
	 * is the initial pixel to transfer and how many pixels are required
	 * to process the next token.
	 * 1. accelerator_if: process all pixels in one burst
	 * 2. circular_buffer: process one row per burst (notice taht to start
	 *                     you must firt read five rows!)
	 *
	 * Your DMAC should do the following (partialy implemented for you)
	 * a. De-activate the DUT reset active
	 * b. Read input.txt to determine input image and its size
	 * c. Configure the DUT with the image size and set conf_done signal
	 * d. Wait for read/write requests and handle them properly
	 *    Use FlexChannels with bufdin and bufdout initators to pass image
	 *    pixels to/from DUT
	 * e. Check for debayer_done(), which should be set when the DUT has
	 *    comleted one full image. At this point the TB must set the DUT
	 *    reset for at least one clock cycle. Next TB should validate the
	 *    output by reading golden output images. Finally TB should start
	 *    over from step a. until all of the images listed in input.txt
	 *    have been processed. Make sure to print a time stamp every time
	 *    one image is complete.
	 * f. Call sc_stop() and print simulation summary. (already implemented)
	 */

	// RESET
  int count = 0;
  int pic_size = 0;
  int burst_rd = 0;
  int index_rd = 0;
  int burst_wr = 0;
  int index_wr = 0;
  std::string test_filename;
  std::string exp_filename;
  
  bufdout.reset_get();
  bufdin.reset_put();
  cout<< "DMA reset @ " << sc_time_stamp() <<endl;
  rst_dut.write(false);
  wait();
  
	while(count < num_samples) {
    
		if (!rst_dut.read()) {
			// Read input and configure
      conf_done.write(false);//like a reset. Let dut ready to read pic
      
      input >> pic_size >> test_filename;
      cout << "size of pic: " << pic_size << " file name: " << test_filename << endl;
      
      rst_dut.write(true);
      conf_size.write(pic_size);
      conf_done.write(true);
   	  read_image_file((void *) bayer, sizeof(u16), test_filename.c_str(),//read image
		  ".", sizeof(u16) * pic_size * pic_size);
        
      cout << "TB after config image and read image @ " << sc_time_stamp() << endl;
     	}   
    
		do { wait(); }
		while (!rd_request.read() && !wr_request.read() && !debayer_done.read()); //wait when idle
   
		if (debayer_done.read()) {
			cout << "TB Debayer done @ " << sc_time_stamp() << endl;
			// Reset DUT

      //???????????????????????????????????????????????????
      rst_dut.write(false);//until after tb config rst over 
      wait();//reset need wait() meaning last some time
      cout << "No. " << count << " image done @ " << sc_time_stamp() << endl;
      cout << "TB reset DUT for at lease one cycle @ " << sc_time_stamp() << endl;
			
      // Write output file and validate
      
      std::stringstream res_filename;
      
      res_filename << "../output/" << std::setw(4) << std::setfill('0') << count << ".bin";
      golden_output >> exp_filename;
      
      cout <<  res_filename.str().c_str()<<endl;
      
  	  write_image_file((void *) debayer, sizeof(u16), res_filename.str().c_str(), ".",
      pic_size-2*PAD,pic_size-2*PAD, 3);
      
      read_image_file((void *) gold_debayer, sizeof(rgb_pixel),
		  exp_filename.c_str(), ".", sizeof(rgb_pixel) * (pic_size-2*PAD) * (pic_size-2*PAD));
        
       
      for (int i = 0; i < (pic_size-2*PAD) * (pic_size-2*PAD); i++) {
			if ((*(gold_debayer+i)).r != (*(debayer+i)).r ||
					(*(gold_debayer+i)).g != (*(debayer+i)).g ||
					(*(gold_debayer+i)).b != (*(debayer+i)).b) {
				mismatches++;
				cout << std::dec << "@ " << sc_time_stamp();
				cout << " : EXPECT: " << exp_filename << endl;
				break;
        
        
			}
      
		}

			// Process next image if any
			count++;
			continue;
		}

		if (rd_request.read()) {
			// Process read request and synchronize with a 4-phases handshake
     
      cout << "TB received rd_request @ " << sc_time_stamp() << endl;
      rd_grant.write(true);//grant high
      
      do{wait();}
      while(rd_request.read());//wait request low
      rd_grant.write(false);//grant low
      
      index_rd = rd_index.read();
      burst_rd = rd_length.read();
      
      for(int i=0;i<burst_rd;i++){//transfer data
       bufdin.put(*(bayer+i+index_rd));  
       wait();
      }
      
      
      
			// Uncomment for debugging (you will need it!)
			// cout << "TB: READ BURST; len = " << length
			//      << "; index = " << index << endl;

			// DMA from memory to DUT

		} else {
			// Process write request and synchronize with a 4-phases handshake
      //request high already
      cout << "An active request from DUT to write = :" << wr_request.read() << endl;
      
      wr_grant.write(true);//grant high
      index_wr = wr_index.read();
      burst_wr = wr_length.read();
      //cout << "burst_wr " << burst_wr << endl;
      //cout << "index_wr " << index_wr << endl;
      
      do{wait();}
      while(wr_request.read());//wait request low
      wr_grant.write(false);//grant low
      
      for(int j=0;j< burst_wr;j++){//transfer data
      //cout << "CHECK_WRITE" << endl;
       *(debayer+j+index_wr) = bufdout.get();  
       wait();
      }
      //cout << "CHECK_WRITE" << endl;
      
      
      cout << "A burst of write is done @ " << sc_time_stamp() << endl;
      
			// Uncomment for debugging (you will need it!)
			// cout << "TB: WRITE BURST; len = " << length
			//      << "; index = " << index << endl;

			// DMA from device to memory
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
