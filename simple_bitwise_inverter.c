#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>

#define DRIVER_NAME "smpinv"

static struct{
	dev_t devnum;
	struct cdev c_dev;
	uint8_t data;
	uint8_t data_read;
} inv_data;

static int inv_convert_data(uint8_t base)
{
	return (base ^ 0xFF);
}

static ssize_t inv_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char cbuf = 0;
	
	if(copy_from_user(&cbuf, buf, 1))
		return -EFAULT;

	inv_data.data = cbuf;

	return count;
}

static ssize_t inv_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	int size;
	if (inv_data.data)
	{
		if (*ppos > 0)
			return 0;

		if (size>count)
			size = count;

		inv_data.data_read = inv_convert_data(inv_data.data);

		if(copy_to_user(buf, &inv_data.data_read, 1))
		{
			pr_err("%s: Error - Print back failed.\n", DRIVER_NAME);
			return -EFAULT;
		}

		*ppos += size;

		pr_info("%s: MSG Printed. Content %02hhx.\n", DRIVER_NAME, inv_data.data);
		return 1;
	}
	return 0;
}

static const struct file_operations inv_fops = {
	.owner = THIS_MODULE,
	.write = inv_write,
	.read  = inv_read,
};

static int __init inv_init(void)
{
	inv_data.data = 0x0;
	inv_data.data_read = 0x0;

	int status;

	status = alloc_chrdev_region(&inv_data.devnum, 0, 1, DRIVER_NAME);
	if (status) 
	{
		pr_err("%s: Error - Failed to allocate device number.\n", DRIVER_NAME);
		return status;
	}

	cdev_init(&inv_data.c_dev, &inv_fops);
	status = cdev_add(&inv_data.c_dev, inv_data.devnum, 1);
	if (status)
	{
		pr_err("%s: Error - Failed to register char device.\n", DRIVER_NAME);
		unregister_chrdev_region(inv_data.devnum, 1);
		return status;
	}

	pr_info("%s: Module Initialized", DRIVER_NAME);
	return 0;
}

static void __exit inv_exit(void)
{
	pr_info("%s: Module Off.\n", DRIVER_NAME);
	cdev_del(&inv_data.c_dev);
	unregister_chrdev_region(inv_data.devnum, 1);
}

module_init(inv_init);
module_exit(inv_exit);

MODULE_LICENSE("GLP");
MODULE_AUTHOR("Michal Stankiewicz");
MODULE_VERSION("1.0.0");
MODULE_DESCRIPTION("Simple Bitwise Inverter");
