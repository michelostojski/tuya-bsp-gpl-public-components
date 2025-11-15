/*
 * sys_sensor.h
 *
 * sensor debug header file
 *
 * Copyright (C) 2020 anyka
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */
#ifndef __SYS_SENSOR_H__
#define __SYS_SENSOR_H__

/*
 * sys_sensor.h
 * Anyka sensor system interface definitions
 *
 * Provides callback structures for sensor parameter and clock access,
 * and declares sensor init/exit functions used by the ISP driver.
 */

#include <linux/types.h>

/* --------------------------------------------------------------
 *  Structures defining sensor callbacks
 * -------------------------------------------------------------- */

/*
 * The original minimal declaration in this header caused
 * "dereferencing pointer to incomplete type" errors because
 * sys_sensor.c expects many callback members (resolution, id, regs, etc.)
 *
 * Provide a callback table matching what sys_sensor.c and video.c use:
 * - callbacks take a 'void *arg' context as first parameter when sys_sensor.c
 *   calls them with sensor_cbi->arg
 * - include the common sensor operations used in this driver subset
 *
 * NOTE: Keep this in sync with the real SDK / include/ak_isp_drv.h prototypes.
 * If your upstream header differs, prefer the upstream header and remove
 * this local copy (or make this header include the upstream one).
 */
struct sensor_cb {
	/* get sensor parameter (uses arg, parameter id, pointer to return value) */
	int (*sensor_get_parameter_func)(void *arg, int param, void *value);

	/* get master clock frequency or register controlling clock */
	unsigned int (*sensor_get_mclk_func)(void *arg);

	/* get bus type (raw / yuv etc.) */
	int (*sensor_get_bus_type_func)(void *arg);

	/* get valid image offset/coordinate */
	int (*sensor_get_valid_coordinate_func)(void *arg, int *left, int *top);

	/* read / write sensor register (I2C/SPI access) */
	int (*sensor_read_reg_func)(void *arg, int reg_addr);
	int (*sensor_write_reg_func)(void *arg, int reg_addr, int value);

	/* read sensor id (take context arg) */
	int (*sensor_read_id_func)(void *arg);

	/* get resolution (width/height) */
	int (*sensor_get_resolution_func)(void *arg, int *width, int *height);

	/* optional: power/on/init hooks (may be NULL) */
	int (*sensor_set_power_on_func)(void *arg, int pwdn_pin, int reset_pin);
	int (*sensor_init_func)(void *arg, void *para);
};

/* Sensor callback wrapper (passed to ISP driver) */
struct sensor_cb_info {
	void *arg;                    /* pointer to sensor-specific context */
	struct sensor_cb *cb;         /* function callback table */
	 int sensor_id;                       
    struct v4l2_subdev *subdev;  
};

/* --------------------------------------------------------------
 *  Sensor system interface functions
 * -------------------------------------------------------------- */
struct sensor_attr;  /* Forward declaration for attributes structure */

int sys_sensor_init(struct sensor_attr *sensor);
int sys_sensor_exit(struct sensor_attr *sensor);

#endif /* __SYS_SENSOR_H__ */
