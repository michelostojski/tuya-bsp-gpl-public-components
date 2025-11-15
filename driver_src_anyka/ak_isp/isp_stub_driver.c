#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVICE_NAME "isp"
#define CLASS_NAME  "isp_class"

static int    major_number;
static struct class*  isp_class  = NULL;
static struct device* isp_device = NULL;

static long isp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    pr_info("[isp_stub] IOCTL called: cmd=0x%08x, arg=0x%08lx\n", cmd, arg);

    // You can handle specific known commands if needed
    return 0;  // Return success for all commands
}

static struct file_operations fops = {
    .owner          = THIS_MODULE,
    .unlocked_ioctl = isp_ioctl,
};

static int __init isp_init(void)
{
    pr_info("[isp_stub] Initializing ISP stub driver...\n");

    // Register a char device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        pr_err("[isp_stub] Failed to register char device\n");
        return major_number;
    }

    // Create class
    isp_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(isp_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("[isp_stub] Failed to create class\n");
        return PTR_ERR(isp_class);
    }

    // Create device node: /dev/isp
    isp_device = device_create(isp_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(isp_device)) {
        class_destroy(isp_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        pr_err("[isp_stub] Failed to create device\n");
        return PTR_ERR(isp_device);
    }

    pr_info("[isp_stub] /dev/%s created successfully\n", DEVICE_NAME);
    return 0;
}

static void __exit isp_exit(void)
{
    device_destroy(isp_class, MKDEV(major_number, 0));
    class_unregister(isp_class);
    class_destroy(isp_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    pr_info("[isp_stub] Module unloaded.\n");
}

module_init(isp_init);
module_exit(isp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI - stub ISP device");
MODULE_DESCRIPTION("Minimal stub for /dev/isp to match ak_rtsp_demo");


