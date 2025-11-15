/*
 * sys_isp.c
 *
 * for debug isp
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
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>
#include <linux/videodev2.h>
#include <linux/vmalloc.h>
#include <linux/dma/ipu-dma.h>

#include <media/v4l2-common.h>
#include <media/v4l2-dev.h>
#include <media/v4l2-ioctl.h>
#include <media/v4l2-async.h>
#include <media/v4l2-device.h>
#include <media/videobuf2-dma-contig.h>
#include "camera.h"
#include "include/ak_isp_drv.h"
#include "include/ak_video_priv_cmd.h"
#include "include_internal/ak_video_priv_cmd_internal.h"
#include "cif.h"

#define NO_ISP "noisp"
#define CACHE_SIZE 64
/* forward-declare ak_isp_get_version if its header is not available */
extern int ak_isp_get_version(char *version, int size);
struct sys_isp_infomation {
	int _3dnr_to_isp_id;
};

static struct sys_isp_infomation sys_isp_info = {
	._3dnr_to_isp_id = 0,
};

/*
 * isp_index_show - show isp current index
 */
static ssize_t isp_index_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int len = 0;

	len = snprintf(buf, PAGE_SIZE, "isp_index:%d\n",
			sys_isp_info._3dnr_to_isp_id);

	return len;
}

/*
 * isp_index_store- store isp current index
 */
static ssize_t isp_index_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	sys_isp_info._3dnr_to_isp_id = simple_strtol(buf, NULL, 10);
	return count;
}

/*
 * isp_version_show- show isp version
 */
static ssize_t isp_version_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	int r = 0;
	int len = 0;
	char version[64] = {0};

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* get version and print to buf */
		ak_isp_get_version(version, sizeof(version));
		printk("Version: %s\n", version);
		len += snprintf(buf + len, PAGE_SIZE - len,
				"version:%s\n", version);
		r = len;
	}

	return r;
}

/*
 * ae_run_info_show - show ae running info
 */
static ssize_t ae_run_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_AE_RUN_INFO ae_stat;
	int r = 0;
	int len = 0;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* printf to buf */
		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {
			/* get ae run info */
			r = ak_isp_vp_get_ae_run_info(&ae_stat);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			/* printf to buf */
			len += snprintf(buf + len, PAGE_SIZE - len, "current_calc_avg_lumi:%d\n",
					(int)ae_stat.current_calc_avg_lumi);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_calc_avg_compensation_lumi:%d\n",
					(int)ae_stat.current_calc_avg_compensation_lumi);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_a_gain:%d\n",
					(int)ae_stat.current_a_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_d_gain:%d\n",
					(int)ae_stat.current_d_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_isp_d_gain:%d\n",
					(int)ae_stat.current_isp_d_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_exp_time:%d\n",
					(int)ae_stat.current_exp_time);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_exp_time_step:%d\n",
					(int)ae_stat.current_exp_time_step);
		}
	}

	r = len;

end:
	return r;
}

/*
 * frame_rate_show - show frame rate
 */
static ssize_t frame_rate_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_FRAME_RATE_ATTR frame_rate;
	int r = 0;
	int len = 0;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* printf to buf */
		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {
			/* get frame rate info */
			r = ak_isp_vp_get_frame_rate(&frame_rate);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			/* printf to buf */
			len += snprintf(buf + len, PAGE_SIZE - len, "hight_light_frame_rate:%d\n",
					(int)frame_rate.hight_light_frame_rate);
			len += snprintf(buf + len, PAGE_SIZE - len, "hight_light_max_exp_time:%d\n",
					(int)frame_rate.hight_light_max_exp_time);
			len += snprintf(buf + len, PAGE_SIZE - len, "low_light_frame_rate:%d\n",
					(int)frame_rate.low_light_frame_rate);
			len += snprintf(buf + len, PAGE_SIZE - len, "low_light_max_exp_time:%d\n",
					(int)frame_rate.low_light_max_exp_time);
			len += snprintf(buf + len, PAGE_SIZE - len, "low_light_to_hight_light_gain:%d\n",
					(int)frame_rate.low_light_to_hight_light_gain);
		}
	}

	r = len;

end:
	return r;
}

/*
 * af_stat_info_show - show af statics info
 */
static ssize_t af_stat_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_AF_STAT_INFO af_stat_info;
	int r = 0;
	int len = 0;
	int i;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* printf to buf */
		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {

			/* get af */
			r = ak_isp_vp_get_af_stat_info(&af_stat_info);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			/* printf to buf */
			for (i = 0; i < 5; i++) {
				len += snprintf(buf + len, PAGE_SIZE - len, "af_statics[%d]:%d\n",
						i, (int)af_stat_info.af_statics[i]);
			}
		}
	}

	r = len;

end:
	return r;
}

/*
 * awb_stat_info_show - show awb statics info
 */
static ssize_t awb_stat_info_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_AWB_STAT_INFO awb_stat_info;
	int r = 0;
	int len = 0;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* printf to buf */
		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {
			/* get awb */
			r = ak_isp_vp_get_awb_stat_info(&awb_stat_info);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			/* printf to buf */
			len += snprintf(buf + len, PAGE_SIZE - len, "r_gain:%d\n",
					(int)awb_stat_info.r_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "g_gain:%d\n",
					(int)awb_stat_info.g_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "b_gain:%d\n",
					(int)awb_stat_info.b_gain);
			len += snprintf(buf + len, PAGE_SIZE - len, "r_offset:%d\n",
					(int)awb_stat_info.r_offset);
			len += snprintf(buf + len, PAGE_SIZE - len, "g_offset:%d\n",
					(int)awb_stat_info.g_offset);
			len += snprintf(buf + len, PAGE_SIZE - len, "b_offset:%d\n",
					(int)awb_stat_info.b_offset);
			len += snprintf(buf + len, PAGE_SIZE - len, "current_colortemp_index:%d\n",
					(int)awb_stat_info.current_colortemp_index);
		}
	}

	r = len;

end:
	return r;
}

/*
 * 3d_nr_stat_info_show - show 3d nr static info
 */
static ssize_t _3d_nr_stat_info_show(struct device *dev,
		struct device_attribute *attr, char *buf, int start, int end)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_3D_NR_STAT_INFO _3d_nr_stat_info;
	int r = 0;
	int len = 0;
	int i, j;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		/* walk through all input */
		isp = &input_video->isp;

		/* must the same id */
		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		/* printf to buf */
		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {

			/* get 3dnr */
			r = ak_isp_vp_get_3d_nr_stat_info(&_3d_nr_stat_info);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			/* printf to buf */
			len += snprintf(buf + len, PAGE_SIZE - len, "MD_level:%d\n",
					(int)_3d_nr_stat_info.MD_level);
			len += snprintf(buf + len, PAGE_SIZE - len, "start-line:%d, end-line:%d\n",
					start, end);
			for (i = start; i <= end; i++) {
				for (j = 0; j < 32; j++) {
					len += snprintf(buf + len, PAGE_SIZE - len, "%d\t",
							(int)_3d_nr_stat_info.MD_stat[i][j]);
				}
				len += snprintf(buf + len, PAGE_SIZE - len, "\n");
			}
		}
	}

	r = len;

end:
	return r;
}

/*
 * 3d_nr_stat_info_0_show - show 3d nr static info
 */
static ssize_t _3d_nr_stat_info_0_11_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return _3d_nr_stat_info_show(dev,
			attr, buf, 0, 11);
}

/*
 * 3d_nr_stat_info_12_23_show - show 3d nr static info
 */
static ssize_t _3d_nr_stat_info_12_23_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return _3d_nr_stat_info_show(dev,
			attr, buf, 12, 23);
}

/*
 * ae_attr_show - show ae attribute
 */
static ssize_t ae_attr_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct ak_camera_dev *ak_cam = dev_get_drvdata(dev);
	struct input_video_attr *input_video;
	struct isp_attr *isp;
	AK_ISP_AE_ATTR ae_attr;
	int r = 0;
	int len = 0;

	if (!ak_cam)
		return 0;

	buf[0] = '\0';
	list_for_each_entry(input_video, &ak_cam->input_head, list) {
		isp = &input_video->isp;

		if (sys_isp_info._3dnr_to_isp_id != isp->isp_id)
			continue;

		len += snprintf(buf + len, PAGE_SIZE - len,
				"isp-%d:%s\n",
				isp->isp_id,
				(isp->isp_struct) ? "working":"noworking");

		if (isp->isp_struct) {
			r = ak_isp_vp_get_ae_attr(&ae_attr);
			if (r) {
				len = snprintf(buf, PAGE_SIZE, "%s\n", NO_ISP);
				goto end;
			}

			len += snprintf(buf + len, PAGE_SIZE - len, "exp_time_max:%d\n",
					(int)ae_attr.exp_time_max);
			len += snprintf(buf + len, PAGE_SIZE - len, "exp_time_min:%d\n",
					(int)ae_attr.exp_time_min);
			len += snprintf(buf + len, PAGE_SIZE - len, "d_gain_max:%d\n",
					(int)ae_attr.d_gain_max);
			len += snprintf(buf + len, PAGE_SIZE - len, "d_gain_min:%d\n",
					(int)ae_attr.d_gain_min);
			len += snprintf(buf + len, PAGE_SIZE - len, "isp_d_gain_max:%d\n",
					(int)ae_attr.isp_d_gain_max);
			len += snprintf(buf + len, PAGE_SIZE - len, "isp_d_gain_min:%d\n",
					(int)ae_attr.isp_d_gain_min);
			len += snprintf(buf + len, PAGE_SIZE - len, "a_gain_max:%d\n",
					(int)ae_attr.a_gain_max);
			len += snprintf(buf + len, PAGE_SIZE - len, "exp_step:%d\n",
					(int)ae_attr.exp_step);
			len += snprintf(buf + len, PAGE_SIZE - len, "exp_stable_range:%d\n",
					(int)ae_attr.exp_stable_range);
		}
	}

	r = len;

end:
	return r;
}

/*
 * dev_attr_isp_index:	isp index
 * dev_attr_isp_version: isp version
 * dev_attr_ae_run_info: ak_isp_vp_get_ae_rn_info
 * dev_attr_frame_rate: ak_isp_vp_get_frame_rate
 * dev_attr_af_stat_info: ak_isp_vp_ge_af_stat_info
 * dev_attr_awb_stat_info: ak_isp_vp_get_awb_stat_info
 * dev_attr_3d_nr_stat_info_0_11: ak_isp_vp_get_3d_nr_stat_info
 * dev_attr_3d_nr_stat_info_12_23: ak_isp_vp_get_3d_nr_stat_info
 * dev_attr_ae_attr:	ae attribute
 */
static DEVICE_ATTR(isp_index, 0644, isp_index_show, isp_index_store);
static DEVICE_ATTR(isp_version, 0644, isp_version_show, NULL);
static DEVICE_ATTR(ae_run_info, 0644, ae_run_info_show, NULL);
static DEVICE_ATTR(frame_rate, 0644, frame_rate_show, NULL);
static DEVICE_ATTR(af_stat_info, 0644, af_stat_info_show, NULL);
static DEVICE_ATTR(awb_stat_info, 0644, awb_stat_info_show, NULL);
static DEVICE_ATTR(3d_nr_stat_info_0_11, 0644, _3d_nr_stat_info_0_11_show, NULL);
static DEVICE_ATTR(3d_nr_stat_info_12_23, 0644, _3d_nr_stat_info_12_23_show, NULL);
static DEVICE_ATTR(ae_attr, 0644, ae_attr_show, NULL);

static struct attribute *isp_attrs[] = {
	&dev_attr_isp_index.attr,
	&dev_attr_isp_version.attr,
	&dev_attr_ae_run_info.attr,
	&dev_attr_frame_rate.attr,
	&dev_attr_af_stat_info.attr,
	&dev_attr_awb_stat_info.attr,
	&dev_attr_3d_nr_stat_info_0_11.attr,
	&dev_attr_3d_nr_stat_info_12_23.attr,
	&dev_attr_ae_attr.attr,
	NULL,
};

static struct attribute_group isp_attr_grp = {
	.name = "isp",
	.attrs = isp_attrs,
};

int sys_isp_init(struct ak_camera_dev *ak_cam)
{
	int r;
	struct device *dev;

	if (!ak_cam)
		return -EINVAL;

	dev = ak_cam->hw.dev;

	sys_isp_info._3dnr_to_isp_id = 0;

	r = sysfs_create_group(&dev->kobj, &isp_attr_grp);
	if (r)
		goto fail;

	return 0;

fail:
	return r;
}

int sys_isp_exit(struct ak_camera_dev *ak_cam)
{
	struct device *dev;

	if (!ak_cam)
		return -EINVAL;

	dev = ak_cam->hw.dev;

	sysfs_remove_group(&dev->kobj, &isp_attr_grp);
	return 0;
}
