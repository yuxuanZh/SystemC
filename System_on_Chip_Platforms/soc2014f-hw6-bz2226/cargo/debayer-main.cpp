extern "C" {
#include <sys/syscall.h>
#include <sys/types.h>
#include <csp/csp.h>

#include "debayer-sync.h"
#include "sync.h"
}

#include <flex_channels.hpp>
#include "debayer-wrapper.hpp"
#include "debayer.hpp"

#include <sc_dma_controller.hpp>

void debayer_main(struct device *dev)
{
struct debayer_sync *debayer_dev = dev_to_debayer(dev);
  sc_clock clk("clk", 1, SC_NS);
  sc_signal<bool>  rst("rst");
  sc_signal<bool>  rst_dut("rst_dut");

  sc_signal<unsigned> rd_index;   // array index (offset from starting address)
  sc_signal<unsigned> rd_length;  // burst length
  sc_signal<bool>     rd_request; // transaction request
  sc_signal<bool>     rd_grant;   // transaction grant

  sc_signal<unsigned> wr_index;   // array index (offset from starting address)
  sc_signal<unsigned> wr_length;  // burst length
  sc_signal<bool>     wr_request; // transaction request
  sc_signal<bool>     wr_grant;   // transaction grant

  /* DMA controller */
  put_get_channel<unsigned long long> dma_phys_addr;
  put_get_channel<unsigned long> dma_len;
  put_get_channel<bool> dma_write;
  put_get_channel<bool> dma_start;
  put_get_channel<u16> dma_in_data;
  put_get_channel<u16> dma_out_data;

  /* MAC */
  put_get_channel<u16> bufdin;
  put_get_channel<u16> bufdout;
  sc_signal<unsigned> conf_size;
  sc_signal<unsigned> conf_num_samples;
  sc_signal<bool>     conf_done;

  // computation complete. Written by store_output
  sc_signal<bool> debayer_done;

  debayer_wrapper wrapper("wrapper", debayer_dev);
  debayer dut("dut");
  sc_dma_controller<u16> dma("dma_controller", &dev->dma_cont);
  int budget_on_loan = 0;
  int budget;

  sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", SC_DO_NOTHING);

  /* wire everything */
  dut.clk(clk);
  dut.rst(rst_dut);
  dut.rd_index(rd_index);
  dut.rd_length(rd_length);
  dut.rd_request(rd_request);
  dut.rd_grant(rd_grant);
  dut.wr_index(wr_index);
  dut.wr_length(wr_length);
  dut.wr_request(wr_request);
  dut.wr_grant(wr_grant);
  dut.bufdout(bufdout);
  dut.bufdin(bufdin);
  dut.conf_size(conf_size);
 // dut.conf_num_samples(conf_num_samples);
  dut.conf_done(conf_done);
  dut.debayer_done(debayer_done);

  wrapper.clk(clk);
  wrapper.rst(rst);
  wrapper.rst_dut(rst_dut);
  wrapper.rd_index(rd_index);
  wrapper.rd_length(rd_length);
  wrapper.rd_request(rd_request);
  wrapper.rd_grant(rd_grant);
  wrapper.wr_index(wr_index);
  wrapper.wr_length(wr_length);
  wrapper.wr_request(wr_request);
  wrapper.wr_grant(wr_grant);
  wrapper.conf_size(conf_size);
  wrapper.conf_num_samples(conf_num_samples); 
  wrapper.conf_done(conf_done);
  wrapper.debayer_done(debayer_done);
  wrapper.out_phys_addr(dma_phys_addr);
  wrapper.out_len(dma_len);
  wrapper.out_write(dma_write);
  wrapper.out_start(dma_start);

  dma.clk(clk);
  dma.rst(rst);
  dma.in_phys_addr(dma_phys_addr);
  dma.in_len(dma_len);
  dma.in_write(dma_write);
  dma.in_start(dma_start);
  dma.in_data(bufdout);
  dma.out_data(bufdin);

  /* run simulation */
  rst.write(false);
  sc_start(10, SC_NS);
  rst.write(true);

  for (;;) {
    if (unlikely(csp_recv_int(dev->sync, &budget)))
      die_errno("%s: recv", __func__);
    budget = sync_budget_update(budget, &budget_on_loan);
    if (!budget)
      continue;
    sc_start(budget, SC_NS);
  }

}
