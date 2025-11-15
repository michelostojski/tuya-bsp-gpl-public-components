#include <linux/module.h>
#include <linux/kernel.h>
#include "sys_sensor.h"
// Stubbed callback structure
static struct sensor_cb *sensor_cb_registered = NULL;

void ak_camera_register_sensor_cb(struct sensor_cb *cb) {
    printk("ISP STUB: sensor_cb %sregistered at %p\n", cb ? "" : "un", cb);
    sensor_cb_registered = cb;
}
EXPORT_SYMBOL(ak_camera_register_sensor_cb);

struct sensor_cb *get_sensor_cb(void) {
    if (sensor_cb_registered) {
        printk("get_sensor_cb: sensor_cb_registered: %p, sensor_id: %d, subdev: %p\n",
               sensor_cb_registered,
               sensor_cb_registered->sensor_id,
               sensor_cb_registered->subdev);
    } else {
        printk("get_sensor_cb: sensor_cb_registered is NULL\n");
    }
    return sensor_cb_registered;
}

EXPORT_SYMBOL(get_sensor_cb);


void ak_isp_ae_work(void) { return; }
EXPORT_SYMBOL(ak_isp_ae_work);
void ak_isp_awb_work(void) { return; }
EXPORT_SYMBOL(ak_isp_awb_work);
void ak_isp_enable_buffer_ch3(void) { return; }
EXPORT_SYMBOL(ak_isp_enable_buffer_ch3);
void ak_isp_enable_buffer_main(void) { return; }
EXPORT_SYMBOL(ak_isp_enable_buffer_main);
void ak_isp_enable_buffer_sub(void) { return; }
EXPORT_SYMBOL(ak_isp_enable_buffer_sub);
int ak_isp_get_bits_width_compat(void) { return 0; }
EXPORT_SYMBOL(ak_isp_get_bits_width_compat);
int ak_isp_get_pclk_polar_compat(void) { return 0; }
EXPORT_SYMBOL(ak_isp_get_pclk_polar_compat);
int ak_isp_get_version(void) { return 0; }
EXPORT_SYMBOL(ak_isp_get_version);
void ak_isp_irq_work_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_irq_work_compat);
void ak_isp_is_continuous_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_is_continuous_compat);
void ak_isp_reload_td_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_reload_td_compat);
void ak_isp_set_ae_fast_struct_default(void) { return; }
EXPORT_SYMBOL(ak_isp_set_ae_fast_struct_default);
void ak_isp_set_isp_capturing_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_set_isp_capturing_compat);
void ak_isp_set_misc_attr_ex(void) { return; }
EXPORT_SYMBOL(ak_isp_set_misc_attr_ex);
void ak_isp_set_td_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_set_td_compat);
void ak_isp_vi_apply_mode_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vi_apply_mode_compat);
void ak_isp_vi_capturing_one(void) { return; }
EXPORT_SYMBOL(ak_isp_vi_capturing_one);
int ak_isp_vi_get_crop(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vi_get_crop);
int ak_isp_vi_get_input_data_format(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vi_get_input_data_format);
void ak_isp_vi_set_crop_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vi_set_crop_compat);
void ak_isp_vi_start_capturing_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vi_start_capturing_compat);
void ak_isp_vi_stop_capturing_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vi_stop_capturing_compat);
void ak_isp_vo_check_irq_status_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_check_irq_status_compat);
void ak_isp_vo_check_update_status(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_check_update_status);
void ak_isp_vo_clear_irq_status_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_clear_irq_status_compat);
void ak_isp_vo_clear_update_status(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_clear_update_status);
void ak_isp_vo_disable_buffer_ch3(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_disable_buffer_ch3);
void ak_isp_vo_disable_buffer_main(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_disable_buffer_main);
void ak_isp_vo_disable_buffer_sub(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_disable_buffer_sub);
void ak_isp_vo_enable_buffer_ch3(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_enable_buffer_ch3);
void ak_isp_vo_enable_buffer_main(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_enable_buffer_main);
void ak_isp_vo_enable_buffer_sub(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_enable_buffer_sub);
void ak_isp_vo_enable_irq_status_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_enable_irq_status_compat);
int ak_isp_vo_enable_target_lines_done(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vo_enable_target_lines_done);
int ak_isp_vo_get_using_frame_ch3_buf_id(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vo_get_using_frame_ch3_buf_id);
int ak_isp_vo_get_using_frame_main_buf_id(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vo_get_using_frame_main_buf_id);
int ak_isp_vo_get_using_frame_sub_buf_id(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vo_get_using_frame_sub_buf_id);
void ak_isp_vo_set_ch3_buffer_addr(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_set_ch3_buffer_addr);
void ak_isp_vo_set_main_buffer_addr(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_set_main_buffer_addr);
void ak_isp_vo_set_main_channel_scale_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_set_main_channel_scale_compat);
void ak_isp_vo_set_sub_buffer_addr(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_set_sub_buffer_addr);
void ak_isp_vo_set_sub_channel_scale_compat(void) { return; }
EXPORT_SYMBOL(ak_isp_vo_set_sub_channel_scale_compat);
int ak_isp_vp_get_3d_nr_stat_info(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_3d_nr_stat_info);
int ak_isp_vp_get_ae_attr(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_ae_attr);
int ak_isp_vp_get_ae_run_info(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_ae_run_info);
int ak_isp_vp_get_af_stat_info(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_af_stat_info);
int ak_isp_vp_get_awb_stat_info(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_awb_stat_info);
int ak_isp_vp_get_frame_rate(void) { return 0; }
EXPORT_SYMBOL(ak_isp_vp_get_frame_rate);
int isp_param_check_working(void) { return 0; }
EXPORT_SYMBOL(isp_param_check_working);
int isp_param_init(void) { return 0; }
EXPORT_SYMBOL(isp_param_init);
int isp_param_process_irq_end(void) { return 0; }
EXPORT_SYMBOL(isp_param_process_irq_end);
int isp_param_process_irq_start(void) { return 0; }
EXPORT_SYMBOL(isp_param_process_irq_start);
int isp_param_register(void) { return 0; }
EXPORT_SYMBOL(isp_param_register);
int isp_param_uninit(void) { return 0; }
EXPORT_SYMBOL(isp_param_uninit);
int isp_param_unregister(void) { return 0; }
EXPORT_SYMBOL(isp_param_unregister);
int isp_stats_init(void) { return 0; }
EXPORT_SYMBOL(isp_stats_init);
int isp_stats_process_irq_end(void) { return 0; }
EXPORT_SYMBOL(isp_stats_process_irq_end);
int isp_stats_process_irq_start(void) { return 0; }
EXPORT_SYMBOL(isp_stats_process_irq_start);
int isp_stats_register(void) { return 0; }
EXPORT_SYMBOL(isp_stats_register);
int isp_stats_uninit(void) { return 0; }
EXPORT_SYMBOL(isp_stats_uninit);
int isp_stats_unregister(void) { return 0; }
EXPORT_SYMBOL(isp_stats_unregister);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stubbed ISP");
MODULE_DESCRIPTION("Stub driver to resolve ak_isp.ko dependencies.");
