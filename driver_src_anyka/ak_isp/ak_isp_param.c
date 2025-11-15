/*
 * Replaced top-of-file includes / fallback defines / ispdrv_init
 * Drop this block into driver_modules/ak_isp/isp_param.c replacing the
 * existing includes and the ispdrv_init function implementation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/* Ensure driver types are available */
#include "include/ak_isp_drv.h"
#include "ak_isp_compat.h"
#include "ak_isp_char.h"

typedef struct sensor_cb_info sensor_cb_info;

/* Guarded fallback ioctl command values used by isp_param.c when the vendor
 * header doesn't provide them. These are placeholders so the switch-case compiles.
 * Replace with real values from vendor headers when available.
 */
#ifndef AK_ISP_SET_ME
#define AK_ISP_SET_ME           0x2000
#define AK_ISP_GET_ME           0x2001
#define AK_ISP_SET_UVNR         0x2002
#define AK_ISP_GET_UVNR         0x2003
#define AK_ISP_PROBE_SENSOR_ID  0x2004
#define AK_ISP_SET_AE_SUSPEND   0x2005
#endif

/* If the driver header doesn't declare these helper functions, keep externs here.
 * AK_ISP_ME_ATTR and AK_ISP_UVNR_ATTR are provided by ak_isp_compat.h fallback
 * or by include/ak_isp_drv.h if present.
 */
extern int ak_isp_vp_set_me_attr(AK_ISP_ME_ATTR *me);
extern int ak_isp_vp_get_me_attr(AK_ISP_ME_ATTR *me);
extern int ak_isp_vp_set_uvnr_attr(AK_ISP_UVNR_ATTR *uvnr);
extern int ak_isp_vp_get_uvnr_attr(AK_ISP_UVNR_ATTR *uvnr);
extern int ak_isp_set_ae_work_suspend(int val);

/* Callback pointer types (match prototypes expected) */
typedef int (*ISPDRV_CB_WORD_READ)(unsigned long, unsigned int *);
typedef int (*ISPDRV_CB_WORD_WRITE)(unsigned long, unsigned int);

/* Basic stubs used as callbacks */
static int isp_word_read(unsigned long addr, unsigned int *val) {
    *val = 0;
    return 0;
}
static int isp_word_write(unsigned long addr, unsigned int val) {
    (void)addr; (void)val;
    return 0;
}

/* -------------------------------------------------------------------------
 *  ISP Driver Initialization and Exit
 * ------------------------------------------------------------------------- */

/* NOTE: this version sets the global cb_word_read / cb_word_write (if present)
 * rather than attempting to write cb.cb_word_read/cb.cb_word_write members that
 * do not exist in AK_ISP_FUNC_CB in this ak_isp_drv.h.
 */
static int ispdrv_init(void *isp_struct, struct sensor_cb_info *sensor_cbi, void *base, int isp_id)
{
    AK_ISP_FUNC_CB cb;
    int ret;

    memset(&cb, 0, sizeof(cb));

    /* Assign low-level word callbacks to the driver-global symbols (if defined). */
#ifdef cb_word_read
    cb_word_read = (ISPDRV_CB_WORD_READ)isp_word_read;
#endif
#ifdef cb_word_write
    cb_word_write = (ISPDRV_CB_WORD_WRITE)isp_word_write;
#endif

    /* New API requires only cb, sensor_cb, and base */
    ret = isp2_module_init(&cb, (AK_ISP_SENSOR_CB *)sensor_cbi, base);

    if (ret) {
        printk(KERN_ERR "ISP: module init failed (%d)\n", ret);
        return ret;
    }

    printk(KERN_INFO "ISP driver init successful (id=%d)\n", isp_id);
    return 0;
}