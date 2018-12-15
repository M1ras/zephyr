/*
 * Copyright (c) 2018 Miras Absar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ICM20948_I2C_H
#define ICM20948_I2C_H

#include <device.h>

struct icm20948_i2c_data {
	struct device *i2c_dev;

	u8_t user_bank;

	s16_t temp;

	s16_t accel_x;
	s16_t accel_y;
	s16_t accel_z;
	u8_t accel_sensitivity;

	s16_t gyro_x;
	s16_t gyro_y;
	s16_t gyro_z;
	u8_t gyro_sensitivity;

	s16_t mag_x;
	s16_t mag_y;
	s16_t mag_z;
}

#endif
