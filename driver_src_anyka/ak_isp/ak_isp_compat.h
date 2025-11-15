#ifndef AK_ISP_COMPAT_H
#define AK_ISP_COMPAT_H

/* Minimal compat header.
 * Include the real driver header for core types and enums (if present),
 * and provide small fallback typedefs for a few missing AK_ISP_* types
 * that some sources expect.
 *
 * NOTE: these fallback typedefs are intentionally small and only provide
 * the fields that isp_param.c reads (enable, threshold). If/when the
 * real driver header defines these types, you'll need to remove these
 * fallbacks to avoid duplicate-definition conflicts.
 */

#include "include/ak_isp_drv.h" /* attempt to get real driver types if available */
#include <linux/types.h>

#ifndef __INPUT_DATA_FORMAT_DEFINED
#define __INPUT_DATA_FORMAT_DEFINED
struct input_data_format { int format; };
#endif

/* Fallback minimal definitions for types not present in include/ak_isp_drv.h.
 * Guard with unique macros so we don't re-define if you later add real defs.
 */
#ifndef _AK_ISP_ME_ATTR_FALLBACK
#define _AK_ISP_ME_ATTR_FALLBACK
typedef struct {
    int enable;
    int threshold;
} AK_ISP_ME_ATTR;
#endif

#ifndef _AK_ISP_UVNR_ATTR_FALLBACK
#define _AK_ISP_UVNR_ATTR_FALLBACK
typedef struct {
    int enable;
    /* add fields later if isp_param.c needs them */
} AK_ISP_UVNR_ATTR;
#endif

/* Compat wrappers camera.c expects (void *ctx signatures). Keep minimal. */
int ak_isp_get_bits_width_compat(void *ctx);
int ak_isp_get_pclk_polar_compat(void *ctx);
int ak_isp_vi_apply_mode_compat(void *ctx, int mode);
int ak_isp_vo_enable_irq_status_compat(void *ctx, int bit);
int ak_isp_vo_enable_target_lines_done(void *ctx, int lines);
int ak_isp_vo_set_main_channel_scale_compat(void *ctx, int width, int height);
int ak_isp_vo_set_sub_channel_scale_compat(void *ctx, int width, int height);
int ak_isp_vo_set_main_buffer_addr(void *ctx, int id, unsigned long yaddr_main_chan_addr);
int ak_isp_vo_set_sub_buffer_addr(void *ctx, int id, unsigned long yaddr_sub_chan_addr);
int ak_isp_vo_set_ch3_buffer_addr(void *ctx, int id, unsigned long yaddr_chan3_addr);
int ak_isp_enable_buffer_main(void *ctx);
int ak_isp_enable_buffer_sub(void *ctx);
int ak_isp_enable_buffer_ch3(void *ctx);
int ak_isp_vo_enable_buffer_main(void *ctx, int id);
int ak_isp_vo_enable_buffer_sub(void *ctx, int id);
int ak_isp_vo_enable_buffer_ch3(void *ctx, int id);
int ak_isp_is_continuous_compat(void *ctx);
int ak_isp_irq_work_compat(void *ctx);
int ak_isp_vo_get_using_frame_main_buf_id(void *ctx);
int ak_isp_vo_get_using_frame_sub_buf_id(void *ctx);
int ak_isp_vo_get_using_frame_ch3_buf_id(void *ctx);
int ak_isp_vo_disable_buffer_main(void *ctx, int id);
int ak_isp_vo_disable_buffer_sub(void *ctx, int id);
int ak_isp_vo_disable_buffer_ch3(void *ctx, int id);
int ak_isp_vi_capturing_one(void *ctx);
int ak_isp_vo_check_update_status(void *ctx);
int ak_isp_vo_clear_update_status(void *ctx, int bit);
int ak_isp_set_isp_capturing_compat(void *ctx, int flag);
int ak_isp_vi_start_capturing_compat(void *ctx, int yuv420_type);
int ak_isp_vi_stop_capturing_compat(void *ctx);
int ak_isp_vi_set_crop_compat(void *ctx, int sx, int sy, int width, int height);
int ak_isp_vo_check_irq_status_compat(void *ctx);
int ak_isp_vo_clear_irq_status_compat(void *ctx, int bit);
int ak_isp_vi_get_crop(void *ctx, int *sx, int *sy, int *width, int *height);
int ak_isp_set_misc_attr_ex(void *ctx, int oneline, int fsden, int hblank, int fsdnum);
int ak_isp_set_ae_fast_struct_default(void *ctx, void *ae_fast);
int ak_isp_set_td_compat(void *ctx);
int ak_isp_reload_td_compat(void *ctx);
int ak_isp_vi_get_input_data_format(void *ctx, struct input_data_format *idf);

#endif /* AK_ISP_COMPAT_H */
#ifndef __AK_ISP_MOTION_DETECT_API__
#define __AK_ISP_MOTION_DETECT_API__

int ak_isp_get_md_array_max_size(void *ctx, int *width, int *height);
int ak_isp_get_mdinfo(void *ctx, void *stat_para, int *w, int *h, int *bs);


#endif


