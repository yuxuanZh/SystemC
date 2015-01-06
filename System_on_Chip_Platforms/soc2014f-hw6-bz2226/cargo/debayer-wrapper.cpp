extern "C" {
#include <csp/csp.h>

#include "util.h"
#include "io.h"
}

#include "debayer-wrapper.hpp"

void debayer_wrapper::ioread32(struct io_req *req, struct io_rsp *rsp)
{
	int reg = req->local_offset >> 2;

	obj_dbg(&debayer->dev.obj, "%s: size %d offset 0x%x\n", __func__, req->size, req->local_offset);

	switch (reg) {
	case DEBAYER_REG_CMD:
		rsp->val = status_reg;
		break;
	case DEBAYER_REG_SRC:
		rsp->val = dma_phys_addr_src;
		break;
	case DEBAYER_REG_DST:
		rsp->val = dma_phys_addr_dst;
		break;
	case DEBAYER_REG_SIZE:
		rsp->val = conf_size.read();
		break;
	case DEBAYER_REG_NUM_SAMPLES:
		rsp->val = conf_num_samples.read();
		break;
	case DEBAYER_REG_MAX_SIZE:
		//rsp->val = BUF_SIZE;
    rsp->val = 128; 
		break;
	case DEBAYER_REG_ID:
		rsp->val = debayer->dev.id;
		break;
	default:
		BUG();
	}
}

void debayer_wrapper::iowrite32(const struct io_req *req, struct io_rsp *rsp)
{
	int reg = req->local_offset >> 2;

	obj_dbg(&debayer->dev.obj, "%s: size %d offset 0x%x val 0x%x\n",
		__func__, req->size, req->local_offset, req->val);

	rsp->val = req->val;

	switch (reg) {
	case DEBAYER_REG_CMD:
	        if (req->val == 1) {
		        BUG_ON((status_reg != 0));
			conf_done.write(true);
			start_fifo.put(true);
		}
		else if (req->val != 0)
		        BUG();
		status_reg = req->val;
		break;
	case DEBAYER_REG_SRC:
		dma_phys_addr_src = req->val;
		break;
	case DEBAYER_REG_DST:
		dma_phys_addr_dst = req->val;
		break;
	case DEBAYER_REG_SIZE:
		conf_size.write(req->val);
		break;
	case DEBAYER_REG_NUM_SAMPLES:
		conf_num_samples.write(req->val);
		break;
	default:
		BUG();
	}
}

void debayer_wrapper::copy_from_dram(u64 index, unsigned length)
{
	obj_dbg(&debayer->dev.obj, "%s\n", __func__);

	/* Byte address */
	out_phys_addr.put(dma_phys_addr_src + (index * sizeof(u16)));
	/* Number of DMA token (templated type). long long int for debayer */
	out_len.put(length);
	out_write.put(false);
	out_start.put(true);
}

void debayer_wrapper::copy_to_dram(u64 index, unsigned length)
{
	obj_dbg(&debayer->dev.obj, "%s\n", __func__);
	out_phys_addr.put(dma_phys_addr_dst + (index * sizeof(u16)));
	out_len.put(length);
	out_write.put(true);
	out_start.put(true);
}

void debayer_wrapper::start()
{
	// RESET DUT
	rst_dut.write(false);
	wait();
	rst_dut.write(true);

	for (;;) {
		start_fifo.get();
		obj_dbg(&debayer->dev.obj, "CTL start\n");
		drive();
		obj_dbg(&debayer->dev.obj, "DEBAYER done\n");
	}
}

void debayer_wrapper::drive()
{
	for (;;) {
		do {
			wait();
		} while (!rd_request.read() && !wr_request.read() && !debayer_done.read())
			;
		if (debayer_done.read()) {
    cout<<"HERE_done_in_wrapper"<<endl;
			rst_dut.write(false);
			wait();
			rst_dut.write(true);
			// Set bits 5:4 to "10" -> accelerator done
		        status_reg &= ~STATUS_RUN;
			status_reg |= STATUS_DONE;
			device_sync_irq_raise(&debayer->dev);
			break;
		}
		if (rd_request.read()) {
			unsigned index = rd_index.read();
			unsigned length = rd_length.read();

			rd_tran_cnt++;
			rd_byte += length * sizeof(u16);

			rd_grant.write(true);

			do { wait(); }
			while (rd_request.read());
			rd_grant.write(false);
			wait();

			copy_from_dram((u64) index, length);

		} else {
			// WRITE REQUEST
			unsigned index = wr_index.read();
			unsigned length = wr_length.read();
			wr_tran_cnt++;
			wr_byte += length * 3 * sizeof(u16);

			wr_grant.write(true);

			do { wait(); }
			while (wr_request.read());
			wr_grant.write(false);
			wait();

			copy_to_dram( index * 3, length * 3);
		}
	}
}

void debayer_wrapper::io()
{
	struct io_req req;
	struct io_rsp rsp;

	for (;;) {
		/*
		 * Most of the time the channel will be empty; we speed things
		 * up by just peeking directly at the queue, avoiding syscalls.
		 * We do this every 10 cycles to simulate faster.
		 */
		wait(10);
		if (csp_channel_is_empty(debayer->dev.io_req))
			continue;

		if (unlikely(io_recv_req(debayer->dev.io_req, &req)))
			die_errno(__func__);

		//cout << req.write << " - " << std::hex << req.local_offset << " ===" << std::dec << (int)req.size << "===" << endl;

		BUG_ON(req.size != 4); /* XXX */

		rsp.local_offset = req.local_offset;
		rsp.size = req.size;

		if (req.write)
			iowrite32(&req, &rsp);
		else
			ioread32(&req, &rsp);
		if (unlikely(io_send_rsp(req.rsp_chan, &rsp)))
			die_errno(__func__);
	}
}
