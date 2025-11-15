#include <linux/module.h>
#include <linux/kernel.h>

extern void dummy_isp_function(void);

static int __init stub_init(void)
{
    printk(KERN_INFO "ISP STUB: loaded.\n");
    dummy_isp_function();
    return 0;
}

static void __exit stub_exit(void)
{
    printk(KERN_INFO "ISP STUB: unloaded.\n");
}

module_init(stub_init);
module_exit(stub_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OSTINGO");
MODULE_DESCRIPTION("ISP Stub Driver");



