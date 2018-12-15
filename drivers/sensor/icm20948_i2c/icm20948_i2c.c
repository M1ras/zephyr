/*
 * Copyright (c) 2018 Miras Absar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <errno.h>

#include <device.h>
#include <i2c.h>
#include <gpio.h>
#include <sensor.h>
#include <drivers/sensors/icm20948.h>
#include "icm20948_i2c.h"

#define LOG_LEVEL CONFIG_DISPLAY_LOG_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(icm20948_i2c);

static int icm20948_i2c_set_user_bank(struct device *dev, u8_t user_bank)
{
	struct icm20948_i2c_data *data;
	struct device *i2c_dev;

	data = dev->driver_data;
	i2c_dev = data->i2c_dev;

	return 0;
}

static struct sensor_driver_api icm20948_i2c_driver = {
	.attr_set = NULL,
	.trigger_set = NULL,
	.sample_fetch = NULL,
	.channel_get = NULL
};

static int icm20948_i2c_init(struct device *dev)
{
	struct icm20948_i2c_data *data;
	u8_t id;

	data = dev->driver_data;

	data->i2c_dev = device_get_binding(ICM20948_I2C_I2C_DEV_NAME);
	if (!data->i2c_dev) {
		LOG_ERR("Couldn't get I2C dev.");
		return -ENODEV;
	}

	if (i2c_reg_read_byte(data->i2c_dev, ICM20948_I2C_I2C_ADDR,
			      ICM20948_REG_WHO_AM_I, &id) != 0) {
		LOG_ERR("Couldn't read chip ID.");
		return -EIO;
	}

	if (id != ICM20948_VAL_WHO_AM_I && id != ICM20648_VAL_WHO_AM_I) {
		LOG_ERR("Invalid chip ID.");
		return -EINVAL;
	}

	if (i2c_reg_update_byte(data->i2c_dev, ICM20948_I2C_I2C_ADDR,
				ICM20948_REG_PWR_MGMT_1, ICM20948_MASK_SLEEP, 0)
	    != 0) {
		LOG_ERR("Couldn't wake up chip.");
		return -EIO;
	}

	if (i2c_reg_update_byte(data->i2c_dev, ICM20948_I2C_I2C_ADDR,
				ICM20948_REG_PWR_MGMT_1, ICM20948_MASK_SLEEP, 0)
	    != 0) {
		LOG_ERR("Couldn't wake up chip.");
		return -EIO;
	}

	if (i2c_reg_update_byte(data->i2c_dev, ICM20948_I2C_I2C_ADDR,
				ICM20948_REG_REG_BANK_SEL,
				ICM20948_MASK_USER_BANK, 1) != 0) {
		LOG_ERR("Couldn't switch to user bank 1.");
		return -EIO;
	}

	// TODO: Split this out
	// if (i2c_reg_update_byte(data->i2c_dev, ICM20948_I2C_I2C_ADDR,
	// 			ICM20948_REG_REG_BANK_SEL,
	// 			ICM20948_MASK_USER_BANK, 2) != 0) {
	// 	LOG_ERR("Couldn't switch to user bank 2.");
	// 	return -EIO;
	// } else {
	// 	data->user_bank = 2;
	// }

	LOG_DBG("ICM-20948 I2C");
	return 0;
}

static struct icm20948_i2c_data icm20948_i2c_data_i;

DEVICE_AND_API_INIT(ICM20948_I2C, ICM20948_I2C_DEV_NAME, icm20948_i2c_init,
		    &icm20948_i2c_data_i, NULL, APPLICATION,
		    CONFIG_SENSOR_INIT_PRIORITY, &icm20948_i2c_driver);
