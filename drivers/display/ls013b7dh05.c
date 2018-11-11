/*
 * Copyright (c) 2018 Miras Absar
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <device.h>
#include <spi.h>
#include <gpio.h>
#include <display.h>
#include <display/ls013b7dh05.h>

#define LOG_LEVEL CONFIG_DISPLAY_LOG_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(ls013b7dh05);

static int ls013b7dh05_write_buf(const struct device *dev, u8_t *buf,
				 size_t spi_buf_size)
{
	struct ls013b7dh05_data *data;
	struct device *scs_dev;
	struct spi_buf spi_buf_i[1];
	struct spi_buf_set spi_buf_set_i;
	int ret_0, ret_1, ret_2;

	data = dev->driver_data;
	scs_dev = data->scs_dev;

	spi_buf_i[0].buf = buf;
	spi_buf_i[0].len = spi_buf_size;

	spi_buf_set_i.buffers = spi_buf_i;
	spi_buf_set_i.count = ARRAY_SIZE(spi_buf_i);

	ret_0 = gpio_pin_write(scs_dev, LS013B7DH05_SCS_GPIO_PIN_NUM, 1);
	if (ret_0 != 0) {
		LOG_ERR("Couldn't write 1 to GPIO dev.");
		return ret_0;
	}

	ret_1 = spi_write(data->spi_dev, &data->spi_conf, &spi_buf_set_i);
	if (ret_1 != 0) {
		LOG_ERR("Couldn't write %d bytes to SPI dev.", spi_buf_size);
		return ret_1;
	}

	ret_2 = gpio_pin_write(scs_dev, LS013B7DH05_SCS_GPIO_PIN_NUM, 0);
	if (ret_2 != 0) {
		LOG_ERR("Couldn't write 0 to GPIO dev.");
		return ret_2;
	}

	return 0;
}

static int ls013b7dh05_clear(const struct device *dev)
{
	u8_t buf[] = {
		LS013B7DH05_UPDATE_MODE | LS013B7DH05_CLEAR_MODE,
		0b00000000,
		0b00000000
	};

	return ls013b7dh05_write_buf(dev, &buf, ARRAY_SIZE(buf));
}

static int ls013b7dh05_blanking_on(const struct device *dev)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ls013b7dh05_blanking_off(const struct device *dev)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ls013b7dh05_write(const struct device *dev, const u16_t x,
			     const u16_t y,
			     const struct display_buffer_descriptor *desc,
			     const void *buf)
{
	u16_t spi_buf_size;
	u8_t *spi_buf;
	int ret_0;

	/* LS013B7DH05 can only draw full-width lines, so it returns
	 * -EINVAL on non-full-width lines. */
	if (x != 0 || desc->width != LS013B7DH05_WIDTH) {
		LOG_ERR("x must be %d and desc->width must be %d.", 0,
			LS013B7DH05_WIDTH);
		return -EINVAL;
	}

	spi_buf_size = 2 + (LS013B7DH05_TX_BUF_WIDTH_BYTES * desc->height);
	LOG_DBG("Allocating %d bytes.", spi_buf_size);
	spi_buf = calloc(spi_buf_size, sizeof(u8_t));
	if (spi_buf == NULL) {
		LOG_ERR("Couldn't allocate %d bytes.", spi_buf_size);
		return -ENOMEM;
	}

	spi_buf[0] = LS013B7DH05_UPDATE_MODE;
	for (u16_t row_i = 0; row_i < desc->height; row_i++) {
		u16_t spi_buf_i = 1 + (LS013B7DH05_TX_BUF_WIDTH_BYTES * row_i);

		spi_buf[spi_buf_i] = ((1 + y + row_i) * 0x0202020202ULL &
			0x010884422010ULL) % 1023;

		memcpy(&spi_buf[1 + spi_buf_i],
			&buf[LS013B7DH05_FRAME_BUF_WIDTH_BYTES * row_i],
			LS013B7DH05_FRAME_BUF_WIDTH_BYTES);
	}

	ret_0 = ls013b7dh05_write_buf(dev, spi_buf, spi_buf_size);
	LOG_DBG("Freeing %d bytes.", spi_buf_size);
	free(spi_buf);
	return ret_0;
}

static int ls013b7dh05_read(const struct device *dev, const u16_t x,
			    const u16_t y,
			    const struct display_buffer_descriptor *desc,
			    void *buf)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static void *ls013b7dh05_get_framebuffer(const struct device *dev)
{
	LOG_ERR("Unsupported");
	return NULL;
}

static int ls013b7dh05_set_brightness(const struct device *dev,
				      const u8_t brightness)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ls013b7dh05_set_contrast(const struct device *dev,
				    const u8_t contrast)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static void ls013b7dh05_get_capabilities(const struct device *dev,
					 struct display_capabilities *caps)
{
	memset(caps, 0, sizeof(struct display_capabilities));
	caps->x_resolution = LS013B7DH05_WIDTH;
	caps->y_resolution = LS013B7DH05_HEIGHT;
	caps->supported_pixel_formats = PIXEL_FORMAT_MONO01;
	caps->screen_info = SCREEN_INFO_MONO_MSB_FIRST;
	caps->current_pixel_format = PIXEL_FORMAT_MONO01;
	caps->current_orientation = DISPLAY_ORIENTATION_NORMAL;
}

static int ls013b7dh05_set_pixel_format(const struct device *dev,
					const enum display_pixel_format fmt)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static int ls013b7dh05_set_orientation(const struct device *dev,
				       const enum display_orientation orien)
{
	LOG_ERR("Unsupported");
	return -ENOTSUP;
}

static struct display_driver_api ls013b7dh05_driver = {
	.blanking_on = ls013b7dh05_blanking_on,
	.blanking_off = ls013b7dh05_blanking_off,
	.write = ls013b7dh05_write,
	.read = ls013b7dh05_read,
	.get_framebuffer = ls013b7dh05_get_framebuffer,
	.set_brightness = ls013b7dh05_set_brightness,
	.set_contrast = ls013b7dh05_set_contrast,
	.get_capabilities = ls013b7dh05_get_capabilities,
	.set_pixel_format = ls013b7dh05_set_pixel_format,
	.set_orientation = ls013b7dh05_set_orientation
};

static int ls013b7dh05_init(struct device *dev)
{
	struct ls013b7dh05_data *data;
	int ret_0;

	data = dev->driver_data;

	data->spi_dev = device_get_binding(LS013B7DH05_SPI_DEV_NAME);
	if (!data->spi_dev) {
		LOG_ERR("Couldn't get SPI dev.");
		return -ENODEV;
	}

	data->spi_conf.frequency = LS013B7DH05_SPI_DEV_FREQ;
	data->spi_conf.operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8);
	data->spi_conf.slave = LS013B7DH05_SPI_DEV_NUM;

	data->scs_dev = device_get_binding(LS013B7DH05_SCS_GPIO_PORT_NAME);
	if (!data->scs_dev) {
		LOG_ERR("Couldn't get GPIO dev.");
		return -ENODEV;
	}

	ret_0 = gpio_pin_configure(data->scs_dev, LS013B7DH05_SCS_GPIO_PIN_NUM,
		GPIO_DIR_OUT);
	if (ret_0 != 0) {
		LOG_ERR("Couldn't config GPIO dev.");
		return ret_0;
	}

	/* LS013B7DH05 inits with a noisy image, so it's cleared after init. */
	return ls013b7dh05_clear(dev);
}

static struct ls013b7dh05_data ls013b7dh05_data_i;

DEVICE_AND_API_INIT(LS013B7DH05, LS013B7DH05_DEV_NAME, ls013b7dh05_init,
		    &ls013b7dh05_data_i, NULL, APPLICATION,
		    CONFIG_APPLICATION_INIT_PRIORITY, &ls013b7dh05_driver);
