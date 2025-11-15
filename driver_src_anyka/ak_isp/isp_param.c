#include "ak_isp_compat.h"

#ifndef AK_ISP_SET_ME
#define AK_ISP_SET_ME           0x2000
#define AK_ISP_GET_ME           0x2001
#define AK_ISP_SET_UVNR         0x2002
#define AK_ISP_GET_UVNR         0x2003
#define AK_ISP_PROBE_SENSOR_ID  0x2004
#define AK_ISP_SET_AE_SUSPEND   0x2005
#endif

typedef struct sensor_cb_info sensor_cb_info;

extern int ak_isp_vp_set_me_attr(AK_ISP_ME_ATTR *me);
extern int ak_isp_vp_get_me_attr(AK_ISP_ME_ATTR *me);
extern int ak_isp_vp_set_uvnr_attr(AK_ISP_UVNR_ATTR *uvnr);
extern int ak_isp_vp_get_uvnr_attr(AK_ISP_UVNR_ATTR *uvnr);
extern int ak_isp_set_ae_work_suspend(int val);

typedef int (*ISPDRV_CB_WORD_READ)(unsigned long, unsigned int *);
typedef int (*ISPDRV_CB_WORD_WRITE)(unsigned long, unsigned int);

static int isp_word_read(unsigned long addr, unsigned int *val) {

    *val = 0;
    return 0;
}
static int isp_word_write(unsigned long addr, unsigned int val) {

    return 0;
}

/*
 * isp_param.c - Corrected compatibility version
 * Works with ak_isp_drv.h V3.1.x and ak_isp_compat.h
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include "ak_isp_drv.h"
#include "ak_isp_compat.h"
#include "ak_isp_char.h"

/* -------------------------------------------------------------------------
 *  Simplified data structures for ME and UVNR support
 * ------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------
 *  Static helper prototypes
 * ------------------------------------------------------------------------- */
static int printk_me_para(AK_ISP_ME_ATTR *me_para)
{

    if (!me_para)
        return -EINVAL;
    printk(KERN_INFO "[ISP] ME enable=%d, threshold=%d\n",
           me_para->enable, me_para->threshold);
    return 0;
}

/* -------------------------------------------------------------------------
 *  ISP Driver Initialization and Exit
 * ------------------------------------------------------------------------- */

static int ispdrv_init(void *isp_struct, struct sensor_cb_info *sensor_cbi, void *base, int isp_id)
{

    AK_ISP_FUNC_CB cb;
    int ret;

    memset(&cb, 0, sizeof(cb));

    /* Assign function callbacks (if available) */
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

static void ispdrv_uninit(void *isp_struct)
{

    isp2_module_fini();
    printk(KERN_INFO "ISP driver uninit done.\n");
}

/* -------------------------------------------------------------------------
 *  IOCTL Dispatcher
 * ------------------------------------------------------------------------- */
static long isp_param_ioctl(void *isp_struct, unsigned int cmd, unsigned long arg)
{

    int ret = 0;

    switch (cmd) {

    /* --- Motion Estimation --- */
    case AK_ISP_SET_ME: {
        AK_ISP_ME_ATTR *me_para;
        me_para = kmalloc(sizeof(AK_ISP_ME_ATTR), GFP_KERNEL);
        if (!me_para)
            return -ENOMEM;

        if (copy_from_user(me_para, (void __user *)arg, sizeof(AK_ISP_ME_ATTR))) {
            kfree(me_para);
            return -EFAULT;
        }

        printk_me_para(me_para);
        ret = ak_isp_vp_set_me_attr(me_para);
        kfree(me_para);
        break;
    }

    case AK_ISP_GET_ME: {
        AK_ISP_ME_ATTR *me_para;
        me_para = kmalloc(sizeof(AK_ISP_ME_ATTR), GFP_KERNEL);
        if (!me_para)
            return -ENOMEM;

        ret = ak_isp_vp_get_me_attr(me_para);
        if (!ret) {
            if (copy_to_user((void __user *)arg, me_para, sizeof(AK_ISP_ME_ATTR)))
                ret = -EFAULT;
        }
        kfree(me_para);
        break;
    }

    /* --- UV Noise Reduction --- */
    case AK_ISP_SET_UVNR: {
        AK_ISP_UVNR_ATTR *uvnr;
        uvnr = kmalloc(sizeof(AK_ISP_UVNR_ATTR), GFP_KERNEL);
        if (!uvnr)
            return -ENOMEM;

        if (copy_from_user(uvnr, (void __user *)arg, sizeof(AK_ISP_UVNR_ATTR))) {
            kfree(uvnr);
            return -EFAULT;
        }

        ret = ak_isp_vp_set_uvnr_attr(uvnr);
        kfree(uvnr);
        break;
    }

    case AK_ISP_GET_UVNR: {
        AK_ISP_UVNR_ATTR *uvnr;
        uvnr = kmalloc(sizeof(AK_ISP_UVNR_ATTR), GFP_KERNEL);
        if (!uvnr)
            return -ENOMEM;

        ret = ak_isp_vp_get_uvnr_attr(uvnr);
        if (!ret) {
            if (copy_to_user((void __user *)arg, uvnr, sizeof(AK_ISP_UVNR_ATTR)))
                ret = -EFAULT;
        }
        kfree(uvnr);
        break;
    }

    /* --- Probe Sensor ID --- */
    case AK_ISP_PROBE_SENSOR_ID: {
        AK_ISP_SENSOR_CB *sensor_cb = (AK_ISP_SENSOR_CB *)isp_struct;
        if (sensor_cb && sensor_cb->sensor_probe_id_func) {
            /* Compatibility fix: newer SDKs do not expect an argument */
#ifdef SENSOR_PROBE_ID_TAKES_ARG
            ret = sensor_cb->sensor_probe_id_func(0);
#else
            ret = sensor_cb->sensor_probe_id_func();
#endif
        } else {
            ret = -ENODEV;
        }
            ret = -ENODEV;
        break;
    }

    /* --- AE Suspend --- */
    case AK_ISP_SET_AE_SUSPEND:
        ret = ak_isp_set_ae_work_suspend((int)arg);
        break;

    default:
        printk(KERN_WARNING "Unknown ISP IOCTL cmd=0x%x\n", cmd);
        ret = -ENOTTY;
        break;
    }

    return ret;
}

/* -------------------------------------------------------------------------
 *  Module hooks
 * ------------------------------------------------------------------------- */
MODULE_DESCRIPTION("Anyka ISP Parameter Control (compat version)");
MODULE_AUTHOR("OSTINGO + ChatGPT AutoFix");
MODULE_LICENSE("GPL");



