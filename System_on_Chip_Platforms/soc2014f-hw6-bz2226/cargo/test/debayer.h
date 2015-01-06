#ifndef _DEBAYER_H_
#define _DEBAYER_H_

#ifdef __KERNEL__
#include <linux/ioctl.h>
#include <linux/types.h>
#else
#include <sys/ioctl.h>
#include <stdint.h>
#ifndef __user
#define __user
#endif
#endif /* __KERNEL__ */

#include "wami_debayer.h"

#define DRV_NAME  "debayer"
#define PFX    DRV_NAME ": "
#define DEBAYER_MAX_DEVICES  64


#define DEBAYER_REG_CMD         0x00
#define DEBAYER_REG_SRC         0x04
#define DEBAYER_REG_DST         0x08
#define DEBAYER_REG_SIZE        0x0c
#define DEBAYER_REG_NUM_SAMPLES 0x10
#define DEBAYER_REG_MAX_SIZE    0x14
#define DEBAYER_REG_ID          0x18

#define DEBAYER_CMD_IRQ_SHIFT  4
#define DEBAYER_CMD_IRQ_MASK   0x3
#define DEBAYER_CMD_IRQ_DONE   0x2

#define DEBAYER_SYNC_DEV_ID    0x2

#define MAX_NUM_SAMPLES    1

/* Determine buffer size (bytes) */
#define DEBAYER_INPUT_SIZE(__sz, __num) (sizeof(u16) * (__sz) * (__sz) * (__num))
#define DEBAYER_OUTPUT_SIZE(__sz, __num) (sizeof(u16) * (__sz - 2*PAD) * (__sz - 2*PAD) * (__num) *3 )

#define DEBAYER_BUF_SIZE(__sz, __num)		\
	(DEBAYER_INPUT_SIZE(__sz, __num) +		\
		DEBAYER_OUTPUT_SIZE(__sz, __num))

struct debayer_access {
	unsigned size;
	unsigned num_samples;
};

#define DEBAYER_IOC_ACCESS     _IOW ('I', 0, struct debayer_access)


#endif /* _DEBAYER_H_ */
