//reference /soc2014/examples/lecture13/MAC_FlexChannels
#include <systemc.h>
#include <ctos_flex_channels.h>

#ifndef __DEBAYER_H__
#define __DEBAYER_H__

#include "wami_params.h"

SC_MODULE(debayer) {

	// Declare I/O ports and initiators here
  sc_in<bool> clk;
  sc_in<bool> rst;
  get_initiator<u16> data_in;
  put_initiator<rgb_pixel> data_out;
  
	// Declare methods which are also SC_CTHREADS here
  void process_debayer();
  void compute_and_clamp_pixel();//transfer via channel not input args
  void compute_and_clamp_pixel_fractional_neg();
	// Implement the module's constructor here
  // Make the appropriate bindings in the constructor
  SC_CTOR(debayer)
    : clk("clk")
    , rst("rst")
    , data_in("data_in")
    , data_out("data_out")
    , pos_out("pos_out")
    , neg_out("neg_out")
    , pixel_in("pixle_in")
    , pos_in("pos_in")
    , neg_in("neg_in")
    , pixel_out("pixel_out")
    , pos_fra_in("pos_fra_in")
    , neg_fra_in("neg_fra_in")
    , pixel_fra_out("pixel_fra_out")
    , pos_fra_out("pos_fra_out")
    , neg_fra_out("neg_fra_out")
    , pixel_fra_in("pixel_fra_in")
    , chan_pos("chan_pos")
    , chan_neg("chan_neg")
    , chan_pixel("chan_pixel")
    , chan_fra_pos("chan_fra_pos")
    , chan_fra_neg("chan_fra_neg")
    , chan_fra_pixel("chan_fra_pixel")
  {
    SC_CTHREAD(process_debayer, clk.pos());
    set_stack_size(0x1000000);
    SC_CTHREAD(compute_and_clamp_pixel, clk.pos());
    set_stack_size(0x1000000);
    SC_CTHREAD(compute_and_clamp_pixel_fractional_neg, clk.pos());
    set_stack_size(0x1000000);
    
    reset_signal_is(rst, false);
    data_in.clk_rst(clk,rst);
    data_out.clk_rst(clk,rst);
    pos_out.clk_rst(clk,rst);
    neg_out.clk_rst(clk,rst);
    pixel_in.clk_rst(clk,rst);
    pos_fra_out.clk_rst(clk,rst);
    neg_fra_out.clk_rst(clk,rst);
    pixel_fra_in.clk_rst(clk,rst);
    //compute_and_clamp_pixel
    pos_in.clk_rst(clk,rst);
    neg_in.clk_rst(clk,rst);
    pixel_out.clk_rst(clk,rst);
    //compute_and_clamp_pixel_fractional_neg
    pos_fra_in.clk_rst(clk,rst);
    neg_fra_in.clk_rst(clk,rst);
    pixel_fra_out.clk_rst(clk,rst);
    //channel
    //main_compute_and_clamp_pixel
    pos_out(chan_pos);
    neg_out(chan_neg);
    pixel_in(chan_pixel);
    pos_in(chan_pos);
    neg_in(chan_neg);
    pixel_out(chan_pixel);
    //main_compute_and_clamp_pixel_fractional_neg
    pos_fra_out(chan_fra_pos);
    neg_fra_out(chan_fra_neg);
    pixel_fra_in(chan_fra_pixel);
    pos_fra_in(chan_fra_pos);
    neg_fra_in(chan_fra_neg);
    pixel_fra_out(chan_fra_pixel);
  }
  
  // Declare here class methods that are not processes.
	// These functions can access member variables
    u16 interp_G_at_RRR_or_G_at_BBB(u32 row,u32 col);//how to use multi dim array as args but here is mem var
    u16 interp_R_at_GRB_or_B_at_GBR(u32 row,u32 col);
    u16 interp_R_at_GBR_or_B_at_GRB(u32 row,u32 col);
    u16 interp_R_at_BBB_or_B_at_RRR(u32 row,u32 col);
    void wami_debayer();

	// Declare member variables here
  // Internal initators and FlexChannels should be declared here as well

  private:
   u16 bayer[WAMI_DEBAYER_IMG_NUM_ROWS][WAMI_DEBAYER_IMG_NUM_COLS];
   rgb_pixel debayered[WAMI_DEBAYER_IMG_NUM_ROWS - 2*PAD][WAMI_DEBAYER_IMG_NUM_COLS - 2*PAD];
   //u16 pos;
   //u16 neg;
   
   //main process
   put_initiator<u16> pos_out;
   put_initiator<u16> neg_out;
   get_initiator<u16> pixel_in;
   put_initiator<u16> pos_fra_out;
   put_initiator<u16> neg_fra_out;
   get_initiator<u16> pixel_fra_in;
   //compute_and_clamp_pixel
   get_initiator<u16> pos_in;
   get_initiator<u16> neg_in;
   put_initiator<u16> pixel_out;
   //compute_and_clamp_pixel_fractional_neg
   get_initiator<u16> pos_fra_in;
   get_initiator<u16> neg_fra_in;
   put_initiator<u16> pixel_fra_out;
   //channel btw main and compute_and_clamp_pixel
   put_get_channel<u16> chan_pos;
   put_get_channel<u16> chan_neg;
   put_get_channel<u16> chan_pixel;
   //channel btw main and compute_and_clamp_pixel_fractional_neg
   put_get_channel<u16> chan_fra_pos;
   put_get_channel<u16> chan_fra_neg;
   put_get_channel<u16> chan_fra_pixel;
   
};

#endif