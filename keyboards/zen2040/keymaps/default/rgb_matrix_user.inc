// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Custom RGB Matrix effects for zen2040. They appear at the end of the effect
// cycle (UG_NEXT / UG_PREV). Both halves compute the same picture: key hits
// reach the secondary half through split.transport.sync.matrix_state, WPM
// through SPLIT_WPM_ENABLE and the effect clock is synced by QMK.

RGB_MATRIX_EFFECT(ZEN_AURORA)
RGB_MATRIX_EFFECT(ZEN_EMBERS)
RGB_MATRIX_EFFECT(ZEN_COMET)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    include "wpm.h"

static uint8_t zen_led_distance(uint8_t a, uint8_t b) {
    int16_t dx = (int16_t)g_led_config.point[a].x - g_led_config.point[b].x;
    int16_t dy = (int16_t)g_led_config.point[a].y - g_led_config.point[b].y;
    uint16_t d = sqrt16((uint16_t)(dx * dx + dy * dy));
    return d > 255 ? 255 : d;
}

static void zen_set_hsv(uint8_t i, uint8_t h, uint8_t s, uint8_t v) {
    hsv_t hsv = {h, s, v};
    rgb_t rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
}

// ---------------------------------------------------------------------------
// ZEN_AURORA: two slow interfering light waves drift across both halves, with
// colors spread +-48 around the selected hue. Typing speed adds energy: the
// faster you type (WPM), the brighter and more saturated the curtain gets.
// Speed = drift speed, hue = base color, brightness = overall level.
// ---------------------------------------------------------------------------
static bool ZEN_AURORA(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t energy = 0;
#    ifdef WPM_ENABLE
    uint8_t wpm = get_current_wpm();
    energy      = wpm >= 100 ? 255 : (uint16_t)wpm * 255 / 100;
#    endif
    uint16_t t = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed / 4, 1));

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t x    = g_led_config.point[i].x;
        uint8_t y    = g_led_config.point[i].y;
        uint8_t w1   = sin8((uint8_t)(x + t));
        uint8_t w2   = sin8((uint8_t)(y * 3 - t * 2 + x / 2));
        uint8_t band = ((uint16_t)w1 + w2) / 2;

        uint8_t h = rgb_matrix_config.hsv.h - 48 + scale8(band, 96);
        uint8_t s = qadd8(scale8(rgb_matrix_config.hsv.s, 200), scale8(energy, 55));
        // Keep brightness in the upper half of the range: WS2812 have only
        // 8-bit PWM, so slow fades through dark values show visible steps.
        // The motion comes mostly from the hue shift instead.
        uint8_t v = qadd8(qadd8(128, scale8(band, 100)), scale8(energy, 27));
        zen_set_hsv(i, h, s, scale8(v, rgb_matrix_config.hsv.v));
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// ---------------------------------------------------------------------------
// ZEN_EMBERS: every key press throws heat into the board around that key.
// Heat cools down over time and glows through a fire palette
// (dark red -> orange -> yellow -> white-hot). Typing fast keeps the whole
// keyboard smouldering; idle keys show only faint flickering embers.
// Speed = how fast it cools down.
// ---------------------------------------------------------------------------
#    define ZEN_EMBERS_SPREAD 40

static uint8_t  zen_heat[RGB_MATRIX_LED_COUNT];
static uint32_t zen_embers_last;
static uint16_t zen_embers_cool_acc;

static void zen_embers_add(uint8_t led) {
    for (uint8_t j = 0; j < RGB_MATRIX_LED_COUNT; j++) {
        uint8_t d = zen_led_distance(led, j);
        if (d < ZEN_EMBERS_SPREAD) {
            zen_heat[j] = qadd8(zen_heat[j], (uint16_t)(ZEN_EMBERS_SPREAD - d) * 110 / ZEN_EMBERS_SPREAD);
        }
    }
    zen_heat[led] = qadd8(zen_heat[led], 150);
}

static void zen_fire_color(uint8_t heat, uint8_t *r, uint8_t *g, uint8_t *b) {
    // 0..85 red rises, 85..170 green joins (orange/yellow), 170..255 blue joins (white-hot)
    *r = heat >= 85 ? 255 : heat * 3;
    *g = heat <= 85 ? 0 : (heat >= 170 ? 255 : (heat - 85) * 3);
    *b = heat <= 170 ? 0 : (heat - 170) * 3;
}

static bool ZEN_EMBERS(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    if (params->init) {
        memset(zen_heat, 0, sizeof(zen_heat));
        zen_embers_last     = g_rgb_timer;
        zen_embers_cool_acc = 0;
    }

    if (params->iter == 0) {
        uint32_t dt     = g_rgb_timer - zen_embers_last;
        zen_embers_last = g_rgb_timer;

        // New hits since the previous frame have a tick no older than dt.
        for (uint8_t k = 0; k < g_last_hit_tracker.count; k++) {
            if (g_last_hit_tracker.tick[k] <= dt) {
                zen_embers_add(g_last_hit_tracker.index[k]);
            }
        }

        // Cooling: 1 heat unit per (3..20 ms), faster with higher speed.
        uint8_t ms_per_unit = 20 - scale8(rgb_matrix_config.speed, 17);
        zen_embers_cool_acc += dt;
        uint8_t cool = 0;
        while (zen_embers_cool_acc >= ms_per_unit && cool < 255) {
            zen_embers_cool_acc -= ms_per_unit;
            cool++;
        }
        for (uint8_t j = 0; j < RGB_MATRIX_LED_COUNT; j++) {
            zen_heat[j] = qsub8(zen_heat[j], cool);
        }

        // Idle sparks so the board never looks dead.
        if (random8() < 6) {
            uint8_t j   = random8_max(RGB_MATRIX_LED_COUNT);
            zen_heat[j] = qadd8(zen_heat[j], 45);
        }
    }

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        uint8_t heat = zen_heat[i];
        if (heat > 20) heat = qadd8(qsub8(heat, 8), random8_max(16)); // flicker
        uint8_t r, g, b;
        zen_fire_color(qadd8(heat, 6), &r, &g, &b);
        uint8_t v = rgb_matrix_config.hsv.v;
        rgb_matrix_set_color(i, scale8(r, v), scale8(g, v), scale8(b, v));
    }
    return rgb_matrix_check_finished_leds(led_max);
}

// ---------------------------------------------------------------------------
// ZEN_COMET: each key press launches two comets from that key, flying left
// and right along its row with a fading tail, across both halves. Every key
// fires its own color (shifted from the selected hue). The board idles in a
// dim glow of the base color.
// Speed = comet speed, hue = base color.
// ---------------------------------------------------------------------------
#    define ZEN_COMET_TAIL 48
#    define ZEN_COMET_ROW_HALF_HEIGHT 7
#    define ZEN_COMET_LIFETIME_MS 1600

static bool ZEN_COMET(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // pixels per second: 60 .. 315
    uint16_t px_per_s = 60 + rgb_matrix_config.speed;

    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        int16_t x = g_led_config.point[i].x;
        int16_t y = g_led_config.point[i].y;

        uint8_t best_v = 0;
        uint8_t best_h = rgb_matrix_config.hsv.h;

        for (uint8_t k = 0; k < g_last_hit_tracker.count; k++) {
            uint16_t tick = g_last_hit_tracker.tick[k];
            if (tick > ZEN_COMET_LIFETIME_MS) continue;
            if (abs(y - (int16_t)g_last_hit_tracker.y[k]) > ZEN_COMET_ROW_HALF_HEIGHT) continue;

            int16_t travel = (uint32_t)tick * px_per_s / 1000;
            int16_t hx     = g_last_hit_tracker.x[k];
            // fade the whole comet over its lifetime
            uint8_t life = 255 - (uint32_t)tick * 255 / ZEN_COMET_LIFETIME_MS;

            for (int8_t dir = -1; dir <= 1; dir += 2) {
                int16_t head   = hx + dir * travel;
                int16_t behind = dir * (head - x); // >0: LED is behind the head
                if (behind < -4 || behind > ZEN_COMET_TAIL) continue;
                uint8_t v = behind <= 0 ? 255 : 255 - (uint16_t)behind * 255 / ZEN_COMET_TAIL;
                v         = scale8(v, life);
                if (v > best_v) {
                    best_v = v;
                    best_h = rgb_matrix_config.hsv.h + g_last_hit_tracker.index[k] * 37;
                }
            }
        }

        uint8_t base = rgb_matrix_config.hsv.v / 6;
        if (best_v > base) {
            zen_set_hsv(i, best_h, rgb_matrix_config.hsv.s, scale8(best_v, rgb_matrix_config.hsv.v));
        } else {
            zen_set_hsv(i, rgb_matrix_config.hsv.h, rgb_matrix_config.hsv.s, base);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
