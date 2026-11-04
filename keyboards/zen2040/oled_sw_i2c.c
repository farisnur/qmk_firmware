// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Bit-banged I2C transport for the SSD1306 OLED.
// On this PCB + RP2040 Pro Micro clone the OLED SDA lands on GP3 and SCL on
// GP2, which the RP2040 hardware I2C blocks cannot route, so drive it in software.

#include "quantum.h"

#ifdef OLED_ENABLE
#include "oled_driver.h"

#ifndef OLED_SW_SDA_PIN
#    define OLED_SW_SDA_PIN GP3
#endif
#ifndef OLED_SW_SCL_PIN
#    define OLED_SW_SCL_PIN GP2
#endif
#ifndef OLED_SW_HALF_PERIOD_US
#    define OLED_SW_HALF_PERIOD_US 1
#endif

#define D() wait_us(OLED_SW_HALF_PERIOD_US)

// Open-drain emulation: release = input with pull-up, assert = drive low.
static inline void rel(pin_t p) { gpio_set_pin_input_high(p); }
static inline void low(pin_t p) { gpio_write_pin_low(p); gpio_set_pin_output(p); }

static void sw_start(void) {
    rel(OLED_SW_SDA_PIN); rel(OLED_SW_SCL_PIN); D();
    low(OLED_SW_SDA_PIN); D();
    low(OLED_SW_SCL_PIN); D();
}

static void sw_stop(void) {
    low(OLED_SW_SDA_PIN); D();
    rel(OLED_SW_SCL_PIN); D();
    rel(OLED_SW_SDA_PIN); D();
}

static bool sw_write(uint8_t b) {
    for (int8_t i = 7; i >= 0; i--) {
        if (b & (1 << i)) rel(OLED_SW_SDA_PIN); else low(OLED_SW_SDA_PIN);
        D();
        rel(OLED_SW_SCL_PIN); D();
        low(OLED_SW_SCL_PIN);
    }
    rel(OLED_SW_SDA_PIN); D();
    rel(OLED_SW_SCL_PIN); D();
    bool ack = !gpio_read_pin(OLED_SW_SDA_PIN);
    low(OLED_SW_SCL_PIN); D();
    return ack;
}

static bool sw_send(uint8_t control, const uint8_t *data, uint16_t size, bool progmem) {
    bool ok = true;
    sw_start();
    ok &= sw_write(OLED_DISPLAY_ADDRESS << 1);
    if (ok && control != 0xFF) ok &= sw_write(control);
    for (uint16_t i = 0; ok && i < size; i++) {
        ok &= sw_write(progmem ? pgm_read_byte(&data[i]) : data[i]);
    }
    sw_stop();
    return ok;
}

void oled_driver_init(void) {
    rel(OLED_SW_SDA_PIN);
    rel(OLED_SW_SCL_PIN);
}

// Command buffers from oled_driver.c already start with the 0x00 control byte.
bool oled_send_cmd(const uint8_t *data, uint16_t size) { return sw_send(0xFF, data, size, false); }
bool oled_send_cmd_P(const uint8_t *data, uint16_t size) { return sw_send(0xFF, data, size, true); }
// Display data needs the 0x40 control byte prepended.
bool oled_send_data(const uint8_t *data, uint16_t size) { return sw_send(0x40, data, size, false); }

#endif // OLED_ENABLE
