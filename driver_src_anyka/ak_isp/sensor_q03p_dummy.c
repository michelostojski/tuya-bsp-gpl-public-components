#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include "sys_sensor.h"  // Ensure it matches your actual sensor_cb definition

// Match exact expected types from sys_sensor.h:
static int dummy_get_parameter(void *arg, void *para) { return 0; }
static int dummy_get_mclk(void *arg) { return 0; }
static int dummy_get_bus_type(void *arg) { return 0; }
static int dummy_get_valid_coordinate(void *arg) { return 0; }
static int dummy_read_reg(void *arg, unsigned int addr, unsigned char *val) { *val = 0xFF; return 0; }
static int dummy_write_reg(void *arg, unsigned int addr, unsigned char val) { return 0; }
static int dummy_read_id(void *arg) { return 0x30; }  // fake sensor ID
static int dummy_get_resolution(void *arg) { return 0; }
static int dummy_set_power_on(void *arg, int on) { return 0; }
static int dummy_init_func(void *arg, void *para) { return 0; }

// Populate only the fields your sys_sensor.h supports
static struct sensor_cb q03p_dummy_cb = {
    .sensor_get_parameter_func        = dummy_get_parameter,
    .sensor_get_mclk_func             = dummy_get_mclk,
    .sensor_get_bus_type_func         = dummy_get_bus_type,
    .sensor_get_valid_coordinate_func = dummy_get_valid_coordinate,
    .sensor_read_reg_func             = dummy_read_reg,
    .sensor_write_reg_func            = dummy_write_reg,
    .sensor_read_id_func              = dummy_read_id,
    .sensor_get_resolution_func       = dummy_get_resolution,
    .sensor_set_power_on_func         = dummy_set_power_on,
    .sensor_init_func                 = dummy_init_func,
};

extern void ak_camera_register_sensor_cb(struct sensor_cb *cb);

static int __init sensor_q03p_dummy_init(void)
{
    ak_camera_register_sensor_cb(&q03p_dummy_cb);
    pr_info("sensor_q03p_dummy: registered dummy sensor_cb\n");
    return 0;
}

static void __exit sensor_q03p_dummy_exit(void)
{
    ak_camera_register_sensor_cb(NULL);
    pr_info("sensor_q03p_dummy: unregistered dummy sensor_cb\n");
}

module_init(sensor_q03p_dummy_init);
module_exit(sensor_q03p_dummy_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("OpenAI Assistant");
MODULE_DESCRIPTION("Fixed dummy sensor for AK ISP");


