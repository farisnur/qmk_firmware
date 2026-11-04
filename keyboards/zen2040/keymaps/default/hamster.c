// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Secondary-half OLED: a hamster in a running wheel driven by typing speed.
// Portrait canvas (OLED_ROTATION_270): 32 x 128 px.
//   y   0..34  wheel + hamster (sleeps with floating "z" when WPM is 0)
//   y  40..55  "WPM" label and value
//   y  72..127 WPM history, one column per 2 s (last ~64 s)

#include QMK_KEYBOARD_H
#include <math.h>
#include "hamster.h"

#define CANVAS_W 32
#define WHEEL_CX 16
#define WHEEL_CY 17
#define WHEEL_R 15
#define WHEEL_BOTTOM 34
#define HAM_X 10
#define HAM_Y 23

#define GRAPH_TOP 72
#define GRAPH_BOTTOM 127
#define GRAPH_MAX_WPM 120
#define GRAPH_SAMPLE_MS 2000

#define FRAME_MS 50

// 12 x 8 sprites, bit 11 = leftmost pixel
static const uint16_t PROGMEM ham_run_a[8] = {0x034, 0x3FC, 0x7FD, 0xFFB, 0xFFE, 0x7FC, 0x208, 0x404};
static const uint16_t PROGMEM ham_run_b[8] = {0x034, 0x3FC, 0x7FD, 0xFFB, 0xFFE, 0x7FC, 0x110, 0x110};
static const uint16_t PROGMEM ham_sleep[8] = {0x000, 0x000, 0x0F8, 0x3FE, 0x7BF, 0xF9F, 0xFFF, 0x7FE};
// 3 x 4 "z", bit 2 = leftmost pixel
static const uint8_t PROGMEM zzz[4] = {0x7, 0x1, 0x2, 0x7};

static void draw_sprite12(const uint16_t *spr, uint8_t x0, uint8_t y0) {
    for (uint8_t y = 0; y < 8; y++) {
        uint16_t row = pgm_read_word(&spr[y]);
        for (uint8_t x = 0; x < 12; x++) {
            oled_write_pixel(x0 + x, y0 + y, row & (1 << (11 - x)));
        }
    }
}

static void draw_z(uint8_t x0, int8_t y0) {
    for (uint8_t y = 0; y < 4; y++) {
        if (y0 + y < 0) continue;
        uint8_t row = pgm_read_byte(&zzz[y]);
        for (uint8_t x = 0; x < 3; x++) {
            if (row & (1 << (2 - x))) oled_write_pixel(x0 + x, y0 + y, true);
        }
    }
}

#define RIM_POINTS 96
static uint8_t rim_x[RIM_POINTS], rim_y[RIM_POINTS];

static void init_rim(void) {
    for (uint8_t i = 0; i < RIM_POINTS; i++) {
        float a  = 2.0f * (float)M_PI * i / RIM_POINTS;
        rim_x[i] = lroundf(WHEEL_CX + WHEEL_R * cosf(a));
        rim_y[i] = lroundf(WHEEL_CY + WHEEL_R * sinf(a));
    }
}

static void draw_wheel(float angle) {
    for (uint8_t y = 0; y <= WHEEL_BOTTOM; y++) {
        for (uint8_t x = 0; x < CANVAS_W; x++) oled_write_pixel(x, y, false);
    }
    for (uint8_t i = 0; i < RIM_POINTS; i++) {
        oled_write_pixel(rim_x[i], rim_y[i], true);
    }
    for (uint8_t k = 0; k < 4; k++) {
        float a = angle + k * (float)M_PI / 2.0f;
        float c = cosf(a), s = sinf(a);
        for (uint8_t r = 2; r < WHEEL_R; r++) {
            oled_write_pixel(lroundf(WHEEL_CX + r * c), lroundf(WHEEL_CY + r * s), true);
        }
    }
}

static uint8_t history[CANVAS_W];

static void draw_graph(void) {
    const uint8_t span = GRAPH_BOTTOM - GRAPH_TOP;
    for (uint8_t x = 0; x < CANVAS_W; x++) {
        uint8_t v = history[x] > GRAPH_MAX_WPM ? GRAPH_MAX_WPM : history[x];
        uint8_t h = (uint16_t)v * span / GRAPH_MAX_WPM;
        for (uint8_t y = GRAPH_TOP; y <= GRAPH_BOTTOM; y++) {
            oled_write_pixel(x, y, y > GRAPH_BOTTOM - h);
        }
    }
}

void render_hamster(void) {
    static uint32_t frame_timer = 0, leg_timer = 0, z_timer = 0, sample_timer = 0;
    static float    angle   = 0;
    static bool     leg_alt = false;
    static uint8_t  z_phase = 0;
    static bool     first   = true;

    if (first) {
        first = false;
        init_rim();
        oled_clear();
        draw_graph();
    }
    if (timer_elapsed32(frame_timer) < FRAME_MS) return;
    frame_timer = timer_read32();

    uint8_t wpm = get_current_wpm();

    if (timer_elapsed32(sample_timer) >= GRAPH_SAMPLE_MS) {
        sample_timer = timer_read32();
        memmove(history, history + 1, CANVAS_W - 1);
        history[CANVAS_W - 1] = wpm;
        draw_graph();
    }

    if (wpm > 0) {
        // 4 spokes are pi/2-symmetric; keep the step well below pi/4 so it never looks reversed
        float step = (wpm > 150 ? 150 : wpm) * 0.004f;
        angle += step;
        if (angle > 2.0f * (float)M_PI) angle -= 2.0f * (float)M_PI;
        uint16_t leg_period = wpm >= 136 ? 60 : 400 - wpm * 5 / 2;
        if (timer_elapsed32(leg_timer) >= leg_period) {
            leg_timer = timer_read32();
            leg_alt   = !leg_alt;
        }
        draw_wheel(angle);
        draw_sprite12(leg_alt ? ham_run_a : ham_run_b, HAM_X, HAM_Y);
    } else {
        if (timer_elapsed32(z_timer) >= 400) {
            z_timer = timer_read32();
            z_phase = (z_phase + 1) & 3;
        }
        draw_wheel(angle);
        draw_sprite12(ham_sleep, HAM_X, HAM_Y);
        draw_z(22, 20 - z_phase);
        draw_z(25, 12 - z_phase);
        draw_z(27, 4 - z_phase);
    }

    oled_set_cursor(0, 5);
    oled_write_P(PSTR(" WPM "), false);
    oled_set_cursor(0, 6);
    oled_write_P(PSTR(" "), false);
    oled_write(get_u8_str(wpm, ' '), false);
    oled_write_P(PSTR(" "), false);
}
