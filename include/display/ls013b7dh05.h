/*
 * Copyright (c) 2018 Miras Absar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LS013B7DH05_H
#define LS013B7DH05_H

#define LS013B7DH05_WIDTH 144
#define LS013B7DH05_HEIGHT 168

#define LS013B7DH05_FRAME_BUF_WIDTH_BYTES 18
#define LS013B7DH05_TX_BUF_WIDTH_BYTES 20

#define LS013B7DH05_UPDATE_MODE 0b10000000
#define LS013B7DH05_INVERT_MODE 0b01000000
#define LS013B7DH05_CLEAR_MODE 0b00100000

#include <device.h>
#include <spi.h>

struct ls013b7dh05_data {
	struct device *spi_dev;
	struct device *scs_dev;
	struct spi_config spi_conf;
};

#endif
