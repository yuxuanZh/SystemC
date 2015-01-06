#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/scatterlist.h>
#include <linux/completion.h>
#include <linux/interrupt.h>
#include <linux/pagemap.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/log2.h>
#include <linux/slab.h>
#include <linux/dad.h>
#include <linux/fs.h>
#include <linux/mm.h>

#include <asm/uaccess.h>

#include "debayer.h"

/* device struct */
struct debayer_device {
	struct cdev cdev;           /* char devices structure */
	struct dad_device *dad_dev; /* parent device */
	struct device *dev;         /* associated device */
	struct module *module;
	struct mutex lock;
	struct completion completion;
	void __iomem *iomem;        /* mmapped registers */
	size_t max_size;            /* Maximum buffer size to be DMA'd to/from in bytes */
	int number;
	//int irq;
};

struct debayer_file {
	struct debayer_device *dev;
	void *vbuf; /* virtual address of physically contiguous buffer */
	dma_addr_t dma_handle; /* physical address of the same buffer */
	size_t size;
};

static const struct dad_device_id debayer_ids[] = {
	{ DEBAYER_SYNC_DEV_ID },
	{ },
};

static struct class *debayer_class;
static dev_t debayer_devno;
static dev_t debayer_n_devices;

static irqreturn_t debayer_irq(int irq, void *dev)
{
	struct debayer_device *debayer = dev;
	u32 cmd_reg;

	cmd_reg = ioread32(debayer->iomem + DEBAYER_REG_CMD);
	cmd_reg >>= DEBAYER_CMD_IRQ_SHIFT;
	cmd_reg &= DEBAYER_CMD_IRQ_MASK;
  //dev_info(debayer->dev,"2222222222222222222222222222");
	if (cmd_reg == DEBAYER_CMD_IRQ_DONE) {
		complete_all(&debayer->completion);
		iowrite32(0, debayer->iomem + DEBAYER_REG_CMD);
   //dev_info(debayer->dev,"333333333333333333333333333");
		return IRQ_HANDLED;
	}
	return IRQ_NONE;
}

static bool debayer_access_ok(struct debayer_device *debayer,
			const struct debayer_access *access)
{
	unsigned max_sz = ioread32(debayer->iomem + DEBAYER_REG_MAX_SIZE);
	if (access->size > max_sz ||
//		access->num_samples > MAX_NUM_SAMPLES ||
		access->size <= 0)
//		access->size <= 0 ||
//		access->num_samples <= 0)
		return false;
   //dev_info(debayer->dev, "444444444444444444444444444444444\n");
	return true;
}

static int debayer_transfer(struct debayer_device *debayer,
			struct debayer_file *file,
			const struct debayer_access *access)
{
	/* compute the input and output burst */
	int wait;

	unsigned sz = access->size;
	unsigned num = access->num_samples;

	size_t in_buf_size = DEBAYER_INPUT_SIZE(sz, num);
	/* size_t out_buf_size = DEBAYER_OUTPUT_SIZE(sz, num); */

	/* printk(KERN_INFO "DEBAYER size: %d\n", sz); */
	/* printk(KERN_INFO "# of samples: %d\n", num); */
	/* printk(KERN_INFO "in_buf_size: %d\n", in_buf_size); */
	/* printk(KERN_INFO "out_buf_size: %d\n", out_buf_size); */

	INIT_COMPLETION(debayer->completion);

	if (!debayer_access_ok(debayer, access))
		return -EINVAL;
//dev_info(debayer->dev, "55555555555555555555555555555555555555555555555\n");
	iowrite32(file->dma_handle, debayer->iomem + DEBAYER_REG_SRC);
	iowrite32(file->dma_handle + in_buf_size, debayer->iomem + DEBAYER_REG_DST);
	iowrite32(access->size, debayer->iomem + DEBAYER_REG_SIZE);
	iowrite32(access->num_samples, debayer->iomem + DEBAYER_REG_NUM_SAMPLES);
	iowrite32(0x1, debayer->iomem + DEBAYER_REG_CMD);
 //dev_info(debayer->dev, "6666666666666666666666666666666666666666666666666\n");

	wait = wait_for_completion_interruptible(&debayer->completion);
 //dev_info(debayer->dev, "777777777777777777777777777777777777777777777777777777\n");
	if (wait < 0){
     
		return -EINTR;}
   //dev_info(debayer->dev, "8888888888888888888888888888888888888888888\n");
	return 0;
} 

static int debayer_access_ioctl(struct debayer_device *debayer,
			struct debayer_file *file,
			void __user *arg)
{
	struct debayer_access access;

	if (copy_from_user(&access, arg, sizeof(access)))
		return -EFAULT;

	if (!debayer_access_ok(debayer, &access))
		return -EINVAL;

	if (mutex_lock_interruptible(&debayer->lock))
		return -EINTR;

	debayer_transfer(debayer, file, &access);
	mutex_unlock(&debayer->lock);

	return 0;
}

static long debayer_do_ioctl(struct file *file, unsigned int cm, void __user *arg)
{
	struct debayer_file *priv = file->private_data;
	struct debayer_device *debayer = priv->dev;

	switch (cm) {
	case DEBAYER_IOC_ACCESS:
		return debayer_access_ioctl(debayer, priv, arg);
	default:
		return -ENOTTY;
	}
}

static long debayer_ioctl(struct file *file, unsigned int cm, unsigned long arg)
{
	return debayer_do_ioctl(file, cm, (void __user *)arg);
}

static int debayer_mmap(struct file *file, struct vm_area_struct *vma)
{
	struct debayer_file *priv = file->private_data;
	struct debayer_device *debayer = priv->dev;
	unsigned long pfn;
	size_t size;

	size = vma->vm_end - vma->vm_start;
	if (size > priv->size) {
		dev_info(debayer->dev, "size: %zu, max size: %zu\n", size, priv->size);
		return -EINVAL;
	}
	pfn = page_to_pfn(virt_to_page(priv->vbuf));
	return remap_pfn_range(vma, vma->vm_start, pfn, size, vma->vm_page_prot);
}

static int debayer_open(struct inode *inode, struct file *file)
{
	struct debayer_file *priv;
	struct debayer_device *debayer;
	int rc;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	debayer = container_of(inode->i_cdev, struct debayer_device, cdev);
	priv->dev = debayer;
	priv->size = debayer->max_size;

	priv->vbuf = dma_alloc_coherent(NULL, priv->size, &priv->dma_handle, GFP_KERNEL);
	if (priv->vbuf == NULL) {
		dev_err(debayer->dev, "cannot allocate contiguous DMA buffer of size %zu\n", priv->size);
		rc = -ENOMEM;
		goto err_dma_alloc;
	}

	if (!try_module_get(debayer->module)) {
		rc = -ENODEV;
		goto err_module_get;
	}

	file->private_data = priv;
	return 0;

err_module_get:
	dma_free_coherent(NULL, priv->size, priv->vbuf, priv->dma_handle);
err_dma_alloc:
	kfree(priv);
	return rc;
}

static int debayer_release(struct inode *inode, struct file *file)
{
	struct debayer_file *priv = file->private_data;
	struct debayer_device *debayer = priv->dev;

	module_put(debayer->module);
	dma_free_coherent(NULL, priv->size, priv->vbuf, priv->dma_handle);
	kfree(priv);
	return 0;
}

/*
 * pointers to functions defined by the driver that perform various operation
 * over the device
 */
static const struct file_operations debayer_fops = {
	.owner           = THIS_MODULE,
	.open            = debayer_open,
	.release         = debayer_release,
	.unlocked_ioctl  = debayer_ioctl,
	.mmap            = debayer_mmap,
};

static int debayer_create_cdev(struct debayer_device *debayer, int ndev)
{
	dev_t devno = MKDEV(MAJOR(debayer_devno), ndev);
	int rc;

	/* char device registration */
	cdev_init(&debayer->cdev, &debayer_fops);
	debayer->cdev.owner = THIS_MODULE;
	rc = cdev_add(&debayer->cdev, devno, 1);
	if (rc) {
		dev_err(debayer->dev, "Error %d adding cdev %d\n", rc, ndev);
		goto out;
	}

	debayer->dev = device_create(debayer_class, debayer->dev, devno, NULL, "debayer.%i", ndev);
	if (IS_ERR(debayer->dev)) {
		rc = PTR_ERR(debayer->dev);
		dev_err(debayer->dev, "Error %d creating device %d\n", rc, ndev);
		debayer->dev = NULL;
		goto device_create_failed;
	}

	dev_set_drvdata(debayer->dev, debayer);
	return 0;

device_create_failed:
	cdev_del(&debayer->cdev);
out:
	return rc;
}

static void debayer_destroy_cdev(struct debayer_device *debayer, int ndev)
{
	dev_t devno = MKDEV(MAJOR(debayer_devno), ndev);

	device_destroy(debayer_class, devno);
	cdev_del(&debayer->cdev);
}

static int debayer_probe(struct dad_device *dev)
{
	struct debayer_device *debayer;
	int dev_id;
	int rc;
	unsigned max_sz;

	debayer = kzalloc(sizeof(*debayer), GFP_KERNEL);
	if (debayer == NULL)
		return -ENOMEM;
	debayer->module = THIS_MODULE;
	debayer->dad_dev = dev;
	debayer->number = debayer_n_devices;
	mutex_init(&debayer->lock);
	init_completion(&debayer->completion);

	debayer->iomem = ioremap(dev->addr, dev->length);
	if (debayer->iomem == NULL) {
		rc = -ENOMEM;
		goto err_ioremap;
	}

	dev_id = ioread32(debayer->iomem + DEBAYER_REG_ID);
	if (dev_id != DEBAYER_SYNC_DEV_ID) {
		rc = -ENODEV;
		goto err_reg_read;
	}

	rc = debayer_create_cdev(debayer, debayer->number);
	if (rc)
		goto err_cdev;

	rc = request_irq(dev->irq, debayer_irq, IRQF_SHARED, "debayer", debayer);
	if (rc) {
		dev_info(debayer->dev, "cannot request IRQ number %d\n", -EMSGSIZE);
		goto err_irq;
	}

	debayer_n_devices++;
	dev_set_drvdata(&dev->device, debayer);

	max_sz = ioread32(debayer->iomem + DEBAYER_REG_MAX_SIZE);
	debayer->max_size = round_up(DEBAYER_BUF_SIZE(max_sz, MAX_NUM_SAMPLES), PAGE_SIZE);
           
	dev_info(debayer->dev, "device registered.\n");
 // dev_info(debayer->dev, "LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLl\n");
	return 0;
err_irq:
	debayer_destroy_cdev(debayer, debayer->number);
err_cdev:
	iounmap(debayer->iomem);
err_reg_read:
	iounmap(debayer->iomem);
err_ioremap:
	kfree(debayer);
	return rc;
}

static void __exit debayer_remove(struct dad_device *dev)
{
	struct debayer_device *debayer = dev_get_drvdata(&dev->device);

	/* free_irq(dev->irq, debayer->dev); */
	debayer_destroy_cdev(debayer, debayer->number);
	iounmap(debayer->iomem);
	kfree(debayer);
	dev_info(debayer->dev, "device unregistered.\n");
}

static struct dad_driver debayer_driver = {
	.probe    = debayer_probe,
	.remove    = debayer_remove,
	.name = DRV_NAME,
	.id_table = debayer_ids,
};

static int __init debayer_sysfs_device_create(void)
{
	int rc;

	debayer_class = class_create(THIS_MODULE, "debayer");
	if (IS_ERR(debayer_class)) {
		printk(KERN_ERR PFX "Failed to create debayer class\n");
		rc = PTR_ERR(debayer_class);
		goto out;
	}

	/*
	 * Dynamically allocating device numbers.
	 *
	 * The major and minor numbers are global variables
	 */
	rc = alloc_chrdev_region(&debayer_devno, 0, DEBAYER_MAX_DEVICES, "debayer");
	if (rc) {
		printk(KERN_ERR PFX "Failed to allocate chrdev region\n");
		goto alloc_chrdev_region_failed;
	}

	return 0;

alloc_chrdev_region_failed:
	class_destroy(debayer_class);
out:
	return rc;
}

static void debayer_sysfs_device_remove(void)
{
	dev_t devno = MKDEV(MAJOR(debayer_devno), 0);

	class_destroy(debayer_class);
	unregister_chrdev_region(devno, DEBAYER_MAX_DEVICES); /* freeing device numbers */
}

/* initialization function */
static int __init debayer_init(void)
{
	int rc;

	printk(KERN_INFO "Device-driver initialization\n");
	rc = debayer_sysfs_device_create();
	if (rc)
		return rc;

	rc = dad_register_driver(&debayer_driver);
	if (rc)
		goto err;

	return 0;

err:
	debayer_sysfs_device_remove();
	return rc;
}

/* shutdown function */
static void __exit debayer_exit(void)
{
	printk(KERN_INFO "Device-driver shutdown\n");
	dad_unregister_driver(&debayer_driver);
	debayer_sysfs_device_remove();
}

module_init(debayer_init) /* register initialization function */
module_exit(debayer_exit) /* register shutdown function */

MODULE_AUTHOR("Paolo Mantovani <paolo@cs.columbia.edu>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("debayer driver");
