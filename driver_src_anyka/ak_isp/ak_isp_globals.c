/* Single-definition file for shared ISP callback globals */

#include "include/ak_isp_drv.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
/* Export global function-pointer variables exactly once.
   They are initialized to NULL and set by the isp init code (if used). */
ISPDRV_CB_WORD_READ  cb_word_read  = NULL;
ISPDRV_CB_WORD_WRITE cb_word_write = NULL;
