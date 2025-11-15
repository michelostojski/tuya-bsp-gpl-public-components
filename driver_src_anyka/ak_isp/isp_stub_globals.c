#include <linux/module.h>
#include <linux/kernel.h>

// Dummy exported symbol
void dummy_isp_function(void)
{
    printk(KERN_INFO "ISP STUB: dummy function called.\n");
}
EXPORT_SYMBOL(dummy_isp_function);



