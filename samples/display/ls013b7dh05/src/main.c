/*
 * Copyright (c) 2018 Miras Absar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <string.h>
#include <display.h>
#include <display/ls013b7dh05.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(main);

void main(void)
{
	struct device *dev;
	u16_t buf_width, buf_height, buf_x, buf_y, buf_size;
	u8_t *buf;
	struct display_buffer_descriptor desc;

	dev = device_get_binding(LS013B7DH05_DEV_NAME);
	buf_width = LS013B7DH05_WIDTH;
	buf_height = 4;
	buf_x = 0;
	buf_y = (LS013B7DH05_HEIGHT - buf_height) / 2;
	buf_size = LS013B7DH05_FRAME_BUF_WIDTH_BYTES * buf_height;
	LOG_DBG("Allocating %d bytes.", buf_size);
	buf = calloc(buf_size, sizeof(u8_t));
	if (buf == NULL) {
		LOG_ERR("Couldn't allocate %d bytes.", buf_size);
		return;
	}

	memset(buf, 255, buf_size);
	buf[LS013B7DH05_FRAME_BUF_WIDTH_BYTES / 2 +
		(LS013B7DH05_FRAME_BUF_WIDTH_BYTES * 0)] = 0b11011011;
	buf[LS013B7DH05_FRAME_BUF_WIDTH_BYTES / 2 +
		(LS013B7DH05_FRAME_BUF_WIDTH_BYTES * 2)] = 0b01111110;
	buf[LS013B7DH05_FRAME_BUF_WIDTH_BYTES / 2 +
		(LS013B7DH05_FRAME_BUF_WIDTH_BYTES * 3)] = 0b10000001;

	desc.buf_size = buf_size;
	desc.width = buf_width;
	desc.height = buf_height;
	desc.pitch = 0;

	display_write(dev, buf_x, buf_y, &desc, buf);
	LOG_DBG("Freeing %d bytes.", buf_size);
	free(buf);
}
