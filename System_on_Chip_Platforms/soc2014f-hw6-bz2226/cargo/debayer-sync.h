#ifndef DEBAYER_SYNC_H
#define DEBAYER_SYNC_H

#include "device.h"

// Registers: must match the list of registers in the driver
enum debayer_regs {
  DEBAYER_REG_CMD,         // Command and status register
                       // Write 0x1 to start computation
                       // Write 0x0 to reset the device
  DEBAYER_REG_SRC,
  DEBAYER_REG_DST,
  DEBAYER_REG_SIZE,        // number of elelments per sample
  DEBAYER_REG_NUM_SAMPLES, // number of samples
  DEBAYER_REG_MAX_SIZE,    // READ-ONLY maximum allowed size
  DEBAYER_REG_ID,          // device ID assigned by OS.
  DEBAYER_NR_REGS,
};

// Status bits in DEBAYER_REG_CMD
#define STATUS_DONE BIT(5); // debayer done
#define STATUS_RUN  BIT(4); // debayer running
                            // error if both set
struct debayer_sync {
  struct device dev;
};

void debayer_main(struct device *dev);

static inline struct debayer_sync *dev_to_debayer(struct device *device)
{
  return container_of(device, struct debayer_sync, dev);
}

static inline struct debayer_sync *obj_to_debayer(struct object *object)
{
  struct device *dev = obj_to_device(object);

  return dev_to_debayer(dev);
}


#endif /* DEBAYER_SYNC_H */
