/* arch/arm/mach-msm/include/mach/board.h
 *
 * Copyright (C) 2007 Google, Inc.
 * Author: Brian Swetland <swetland@google.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef __ASM_ARCH_MSM_BOARD_H
#define __ASM_ARCH_MSM_BOARD_H

#include <linux/types.h>

/* platform device data structures */
struct msm_acpu_clock_platform_data
{
	uint32_t acpu_switch_time_us;
	uint32_t max_speed_delta_khz;
	uint32_t vdd_switch_time_us;
	unsigned long mpll_khz;
	unsigned long power_collapse_khz;
	unsigned long wait_for_irq_khz;
        unsigned int max_vdd;	// nHAVS
        int (*acpu_set_vdd) (int mvolts); // nHAVS
};

struct msm_camera_io_ext {
	uint32_t mdcphy;
	uint32_t mdcsz;
	uint32_t appphy;
	uint32_t appsz;
};

struct msm_camera_device_platform_data {
	void (*camera_gpio_on) (void);
	void (*camera_gpio_off)(void);
	struct msm_camera_io_ext ioext;
};

struct msm_camera_legacy_device_platform_data {
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	void (*config_gpio_on) (void);
	void (*config_gpio_off)(void);
};

#define MSM_CAMERA_FLASH_NONE 0
#define MSM_CAMERA_FLASH_LED  1

struct camera_flash_cfg {
	int num_flash_levels;
	int (*camera_flash)(int level);
	uint16_t low_temp_limit;
	uint16_t low_cap_limit;
	uint8_t postpone_led_mode;
};

#ifdef CONFIG_MSM_CAMERA_OLD
struct msm_camera_sensor_info {
	const char *sensor_name;
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	int mclk;
	int num_flash_levels;
	int (*camera_flash)(int level);
	int need_suspend;
	struct msm_camera_device_platform_data *pdata;
	struct resource *resource;
	uint8_t num_resources;
};
#else
struct msm_camera_sensor_info {
	const char *sensor_name;
	int sensor_reset;
	int sensor_pwd;
	int vcm_pwd;
	void(*camera_clk_switch)(void);
	int mclk;
	int need_suspend;
	struct msm_camera_device_platform_data *pdata;
	struct resource *resource;
	uint8_t num_resources;
	uint32_t waked_up;
	wait_queue_head_t event_wait;
	uint32_t kpi_sensor_start;
	uint32_t kpi_sensor_end;
	int flash_type; /* for back support */
	struct camera_flash_cfg* flash_cfg;
	int (*camera_power_on)(void);
	int (*camera_power_off)(void);
};
#endif

struct snd_endpoint {
	int id;
	const char *name;
};

struct msm_snd_endpoints {
	struct snd_endpoint *endpoints;
	unsigned num;
};

/* common init routines for use by arch/arm/mach-msm/board-*.c */

void __init msm_add_devices(void);
void __init msm_map_common_io(void);
void __init msm_init_irq(void);
void __init msm_clock_init(void);
void __init msm_acpu_clock_init(struct msm_acpu_clock_platform_data *);

#ifdef CONFIG_USB_MSM_72K
void msm_hsusb_set_vbus_state(int online);
/* START: add USB connected notify function */
struct t_usb_status_notifier{
	struct list_head notifier_link;
	const char *name;
	void (*func)(int online);
};
	int usb_register_notifier(struct t_usb_status_notifier *);
	static LIST_HEAD(g_lh_usb_notifier_list);
/* END: add USB connected notify function */
#else
static inline void msm_hsusb_set_vbus_state(int online) {}
#endif
int board_mfg_mode(void);
#endif
