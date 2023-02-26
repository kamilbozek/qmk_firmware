// Copyright 2022 Kamil Bozek (@kamilbozek)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
#define I2C_DRIVER I2CD2
#define I2C1_SDA_PIN GP6
#define I2C1_SCL_PIN GP7
#define OLED_DISPLAY_128X64