#
# Copyright (c) 2018 Miras Absar
#
# SPDX-License-Identifier: Apache-2.0
#

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_ICM20948_I2C icm20948_i2c.c)
zephyr_library_sources_ifdef(CONFIG_ICM20948_I2C_TRIGGER icm20948_i2c_trigger.c)
