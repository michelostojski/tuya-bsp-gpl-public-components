/*
 * isp_param_dev_stub.c
 *
 * Minimal helper that creates /dev/isp-param-0 so userland can open the
 * isp-param device even when the real isp_param implementation is not present.
 *
 * Build: add "obj-m += isp_param_dev_stub.o" to your module Makefile and run
 *        make -C /path/to/kernel M=$(PWD) modules
 *
 * Load order on target:
 *   insmod isp_stub_symbols.ko     # earlier stub that registers ak_camera stuff
 *   insmod sensor_q03p_dummy.ko    # or real sensor module (registers sensor_cb)
 *   insmod isp_param_dev_stub.ko   # creates /dev/isp-param-0
 *   insmod ak_isp.ko               # then load ISP module (or reload if already loaded)
 *
 * This is a test-only helper. Replace with the actual isp_param implementation later.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

static int isp_param_open(struct inode *inode, struct file *file)
{
	pr_info("isp_param_dev_stub: open\n");
	return 0;
}
static int isp_param_release(struct inode *inode, struct file *file)
{
	pr_info("isp_param_dev_stub: release\n");
	return 0;
}
static ssize_t isp_param_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	/* no meaningful data - return EOF */
	return 0;
}
static ssize_t isp_param_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	/* accept and discard */
	return count;
}

static const struct file_operations isp_param_fops = {
	.owner = THIS_MODULE,
	.open = isp_param_open,
	.release = isp_param_release,
	.read = isp_param_read,
	.write = isp_param_write,
};

static struct miscdevice isp_param_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "isp-param-0",
	.fops = &isp_param_fops,
};

static int __init isp_param_dev_stub_init(void)
{
	int ret;

	ret = misc_register(&isp_param_misc);
	if (ret) {
		pr_err("isp_param_dev_stub: misc_register failed: %d\n", ret);
		return ret;
	}
	pr_info("isp_param_dev_stub: registered device /dev/%s\n", isp_param_misc.name);
	return 0;
}

static void __exit isp_param_dev_stub_exit(void)
{
	misc_deregister(&isp_param_misc);
	pr_info("isp_param_dev_stub: unregistered device /dev/%s\n", isp_param_misc.name);
}

module_init(isp_param_dev_stub_init);
module_exit(isp_param_dev_stub_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("helper");
MODULE_DESCRIPTION("Stub module creating /dev/isp-param-0 for ak_isp testing");