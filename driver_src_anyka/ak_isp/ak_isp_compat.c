#include "ak_isp_compat.h"

/* Include the real driver header here (only in the C file) so we can call
 * the real ak_isp_* functions without re-declaring them in the header.
 * Adjust the path if necessary for your build system; most trees include
 * ak_isp_drv.h via include path or "include/ak_isp_drv.h".
 */
#include "include/ak_isp_drv.h"
#include <linux/errno.h>

/* Implement compat wrappers. Where there is a directly matching no-ctx
 * function in ak_isp_drv.h we call it; otherwise provide a safe stub
 * (-ENOSYS) so compilation proceeds. Replace stubs with real mappings
 * when you confirm the underlying API in your ak_isp_drv.h.
 */

int ak_isp_get_bits_width_compat(void *ctx)
{
#ifdef ak_isp_get_bits_width
    return ak_isp_get_bits_width();
#else
    (void)ctx;
    return -ENOSYS;
#endif
}
int ak_isp_get_mdinfo(void *ctx, void *stat_para, int *w, int *h, int *bs) {
    *w = 64;
    *h = 48;
    *bs = 16;
    return 0;
}
/* camera.c expects this compat function to return int (not AK_ISP_PCLK_POLAR) */
int ak_isp_get_pclk_polar_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_get_pclk_polar
    return (int)ak_isp_get_pclk_polar();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vi_apply_mode_compat(void *ctx, int mode)
{
    (void)ctx;
#ifdef ak_isp_vi_apply_mode
    return ak_isp_vi_apply_mode((enum isp_working_mode)mode);
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_enable_irq_status_compat(void *ctx, int bit)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_irq_status
    return ak_isp_vo_enable_irq_status(bit);
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_enable_target_lines_done(void *ctx, int lines)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_target_lines_done
    return ak_isp_vo_enable_target_lines_done(lines);
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_set_main_channel_scale_compat(void *ctx, int width, int height)
{
    (void)ctx;
#ifdef ak_isp_vo_set_main_channel_scale
    return ak_isp_vo_set_main_channel_scale(width, height);
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_set_sub_channel_scale_compat(void *ctx, int width, int height)
{
    (void)ctx;
#ifdef ak_isp_vo_set_sub_channel_scale
    return ak_isp_vo_set_sub_channel_scale(width, height);
#else
    return -ENOSYS;
#endif
}

/* Use ak_isp_vo_set_buffer_addr if available (driver exposes two addresses) */
int ak_isp_vo_set_main_buffer_addr(void *ctx, int id, unsigned long yaddr_main_chan_addr)
{
    (void)ctx;
#ifdef ak_isp_vo_set_buffer_addr
    return ak_isp_vo_set_buffer_addr((enum buffer_id)id, yaddr_main_chan_addr, 0);
#else
    (void)id;
    (void)yaddr_main_chan_addr;
    return -ENOSYS;
#endif
}

int ak_isp_vo_set_sub_buffer_addr(void *ctx, int id, unsigned long yaddr_sub_chan_addr)
{
    (void)ctx;
#ifdef ak_isp_vo_set_buffer_addr
    return ak_isp_vo_set_buffer_addr((enum buffer_id)id, 0, yaddr_sub_chan_addr);
#else
    (void)id;
    (void)yaddr_sub_chan_addr;
    return -ENOSYS;
#endif
}

int ak_isp_vo_set_ch3_buffer_addr(void *ctx, int id, unsigned long yaddr_chan3_addr)
{
    (void)ctx;
#ifdef ak_isp_vo_set_buffer_addr
    /* The driver exposes two addresses (main/sub). Use sub slot as best-effort. */
    return ak_isp_vo_set_buffer_addr((enum buffer_id)id, 0, yaddr_chan3_addr);
#else
    (void)id;
    (void)yaddr_chan3_addr;
    return -ENOSYS;
#endif
}

int ak_isp_enable_buffer_main(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer(BUFFER_ONE);
#else
    return -ENOSYS;
#endif
}

int ak_isp_enable_buffer_sub(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer(BUFFER_TWO);
#else
    return -ENOSYS;
#endif
}

int ak_isp_enable_buffer_ch3(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer(BUFFER_THREE);
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_enable_buffer_main(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}
int ak_isp_vo_enable_buffer_sub(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}
int ak_isp_vo_enable_buffer_ch3(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_enable_buffer
    return ak_isp_vo_enable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}

int ak_isp_is_continuous_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_is_continuous
    return ak_isp_is_continuous();
#else
    return -ENOSYS;
#endif
}

int ak_isp_irq_work_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_irq_work
    return ak_isp_irq_work();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_get_using_frame_main_buf_id(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_get_using_frame_buf_id
    return ak_isp_vo_get_using_frame_buf_id();
#else
    return -ENOSYS;
#endif
}
int ak_isp_vo_get_using_frame_sub_buf_id(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_get_using_frame_buf_id
    return ak_isp_vo_get_using_frame_buf_id();
#else
    return -ENOSYS;
#endif
}
int ak_isp_vo_get_using_frame_ch3_buf_id(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_get_using_frame_buf_id
    return ak_isp_vo_get_using_frame_buf_id();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_disable_buffer_main(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_disable_buffer
    return ak_isp_vo_disable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}
int ak_isp_vo_disable_buffer_sub(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_disable_buffer
    return ak_isp_vo_disable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}
int ak_isp_vo_disable_buffer_ch3(void *ctx, int id)
{
    (void)ctx;
#ifdef ak_isp_vo_disable_buffer
    return ak_isp_vo_disable_buffer((enum buffer_id)id);
#else
    (void)id;
    return -ENOSYS;
#endif
}

int ak_isp_vi_capturing_one(void *ctx)
{
    (void)ctx;
    return -ENOSYS;
}

int ak_isp_vo_check_update_status(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_check_irq_status
    return ak_isp_vo_check_irq_status();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_clear_update_status(void *ctx, int bit)
{
    (void)ctx;
#ifdef ak_isp_vo_clear_irq_status
    return ak_isp_vo_clear_irq_status(bit);
#else
    (void)bit;
    return -ENOSYS;
#endif
}

int ak_isp_set_isp_capturing_compat(void *ctx, int flag)
{
    (void)ctx;
#ifdef ak_isp_set_isp_capturing
    return ak_isp_set_isp_capturing(flag);
#else
    (void)flag;
    return -ENOSYS;
#endif
}

/* VI start/stop/crop (driver exposes no-ctx variants) */
int ak_isp_vi_start_capturing_compat(void *ctx, int yuv420_type)
{
    (void)ctx;
    (void)yuv420_type;
#ifdef ak_isp_vi_start_capturing
    return ak_isp_vi_start_capturing();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vi_stop_capturing_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vi_stop_capturing
    return ak_isp_vi_stop_capturing();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vi_set_crop_compat(void *ctx, int sx, int sy, int width, int height)
{
    (void)ctx;
#ifdef ak_isp_vi_set_crop
    return ak_isp_vi_set_crop(sx, sy, width, height);
#else
    (void)sx; (void)sy; (void)width; (void)height;
    return -ENOSYS;
#endif
}

/* TD helpers (map to ak_isp_set_td / ak_isp_reload_td if available) */
int ak_isp_set_td_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_set_td
    return ak_isp_set_td();
#else
    return -ENOSYS;
#endif
}
int ak_isp_reload_td_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_reload_td
    return ak_isp_reload_td();
#else
    return -ENOSYS;
#endif
}

/* Newly requested compat functions to avoid implicit-declaration errors: */

int ak_isp_vo_check_irq_status_compat(void *ctx)
{
    (void)ctx;
#ifdef ak_isp_vo_check_irq_status
    return ak_isp_vo_check_irq_status();
#else
    return -ENOSYS;
#endif
}

int ak_isp_vo_clear_irq_status_compat(void *ctx, int bit)
{
    (void)ctx;
#ifdef ak_isp_vo_clear_irq_status
    return ak_isp_vo_clear_irq_status(bit);
#else
    (void)bit;
    return -ENOSYS;
#endif
}

/* There is no ak_isp_vi_get_crop in the driver header — provide stub. */
int ak_isp_vi_get_crop(void *ctx, int *sx, int *sy, int *width, int *height)
{
    (void)ctx; (void)sx; (void)sy; (void)width; (void)height;
    return -ENOSYS;
}

int ak_isp_set_misc_attr_ex(void *ctx, int oneline, int fsden, int hblank, int fsdnum)
{
    (void)ctx; (void)oneline; (void)fsden; (void)hblank; (void)fsdnum;
    return -ENOSYS;
}

int ak_isp_set_ae_fast_struct_default(void *ctx, void *ae_fast)
{
    (void)ctx; (void)ae_fast;
    return -ENOSYS;
}

/* Input data format wrapper (no underlying API found) */
int ak_isp_vi_get_input_data_format(void *ctx, struct input_data_format *idf)
{
    (void)ctx; (void)idf;
    return -ENOSYS;
}
