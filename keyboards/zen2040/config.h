// Project Zen rev2 PCB driven by an RP2040 "Pro Micro" footprint board.
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Handedness is stored in EEPROM: flash the *_left.uf2 to the left half
// and *_right.uf2 to the right half once. Afterwards either half may be
// plugged into USB.
#define EE_HANDS

// No VBUS sense pin on generic RP2040 Pro Micro clones: detect master by
// USB enumeration instead (default when USB_VBUS_PIN is not defined).
#define SPLIT_USB_DETECT
#define SPLIT_USB_TIMEOUT 2000
// Watchdog (split.transport.watchdog in keyboard.json) reboots a half that
// thinks it is the secondary but gets no traffic, e.g. both halves ended up
// as secondary after host sleep.

// Double-tap RESET to enter the UF2 bootloader.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// OLED: SDA on GP3, SCL on GP2 (swapped vs. hardware I2C1), driven by the
// bit-banged transport in oled_sw_i2c.c.
#define OLED_SW_SDA_PIN GP3
#define OLED_SW_SCL_PIN GP2
#define OLED_FONT_H "glcdfont.c"
#define OLED_TIMEOUT 60000

// Power budget: 2 x 34 LEDs are fed from one USB port (0.5-0.9 A), and the
// far half's share flows through the TRRS contacts. ~120/255 keeps full white
// around 1 A worst case; colored effects draw far less.
#define RGBLIGHT_LIMIT_VAL 120
