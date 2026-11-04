// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Keyboard-level OLED: both displays run in portrait (OLED_ROTATION_270,
// 32 x 128 px = 5 chars x 16 lines). The USB (master) half shows the status
// screen below; what the other half shows is up to the keymap.

#include "quantum.h"

#ifdef RGB_MATRIX_ENABLE
// LED index <-> key mapping, measured with the ledtest keymap.
// Per half (31 LEDs): rows 0-3 cols 0-5 (outer edge first), row 4 cols 0-4,
// then the two thumb-column pairs (each pair of LEDs is wired to one data line).
// The inner key of row 3 (next to the encoder) has no LED.
// clang-format off
led_config_t g_led_config = {
    {
        {      0,      1,      2,      3,      4,      5, NO_LED },
        {      6,      7,      8,      9,     10,     11, NO_LED },
        {     12,     13,     14,     15,     16,     17,     30 },
        {     18,     19,     20,     21,     22,     23,     29 },
        {     24,     25,     26,     27,     28,     29,     30 },
        {     31,     32,     33,     34,     35,     36, NO_LED },
        {     37,     38,     39,     40,     41,     42, NO_LED },
        {     43,     44,     45,     46,     47,     48,     61 },
        {     49,     50,     51,     52,     53,     54,     60 },
        {     55,     56,     57,     58,     59,     60,     61 },
    }, {
        {  4,  0}, { 18,  0}, { 33,  0}, { 47,  0}, { 61,  0}, { 76,  0}, {  0, 12}, { 18, 12}, { 33, 12}, { 47, 12}, { 61, 12}, { 76, 12}, {  0, 23}, { 18, 23}, { 33, 23}, { 47, 23}, { 61, 23}, { 76, 23}, {  0, 35}, { 18, 35}, { 33, 35}, { 47, 35}, { 61, 35}, { 76, 35}, {  0, 47}, { 18, 47}, { 33, 47}, { 47, 47}, { 61, 47}, { 83, 58}, { 98, 58},
        {220,  0}, {206,  0}, {191,  0}, {177,  0}, {163,  0}, {148,  0}, {224, 12}, {206, 12}, {191, 12}, {177, 12}, {163, 12}, {148, 12}, {224, 23}, {206, 23}, {191, 23}, {177, 23}, {163, 23}, {148, 23}, {224, 35}, {206, 35}, {191, 35}, {177, 35}, {163, 35}, {148, 35}, {224, 47}, {206, 47}, {191, 47}, {177, 47}, {163, 47}, {141, 58}, {126, 58},
    }, {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
    }
};
// clang-format on
#endif

#ifdef OLED_ENABLE

// Layer label, exactly 5 characters so it fills one display line.
__attribute__((weak)) const char *layer_name_user(uint8_t layer) {
    return PSTR("?????");
}

#    include "os_icons.h"

// Host OS icon: 32 x 32 px bitmap across the full display width (lines 0-3).
static void render_os_icon(void) {
    const uint32_t *icon = icon_other;
#    ifdef OS_DETECTION_ENABLE
    switch (detected_host_os()) {
        case OS_MACOS:
        case OS_IOS:
            icon = icon_apple;
            break;
        case OS_WINDOWS:
            icon = icon_windows;
            break;
        case OS_LINUX:
            icon = icon_linux;
            break;
        default:
            break;
    }
#    endif
    for (uint8_t y = 0; y < 32; y++) {
        uint32_t row = pgm_read_dword(&icon[y]);
        for (uint8_t x = 0; x < 32; x++) {
            oled_write_pixel(x, y, row & (1UL << (31 - x)));
        }
    }
}

__attribute__((weak)) void render_status(void) {
    render_os_icon();

    oled_set_cursor(0, 5);
    oled_write_P(PSTR("Layer"), false);
    oled_set_cursor(0, 6);
    oled_write_P(layer_name_user(get_highest_layer(layer_state | default_layer_state)), false);

    led_t leds = host_keyboard_led_state();
    oled_set_cursor(0, oled_max_lines() - 3);
    oled_write_P(leds.num_lock ? PSTR("NUMLK") : PSTR("     "), false);
    oled_write_P(leds.caps_lock ? PSTR("CAPLK") : PSTR("     "), false);
    oled_write_P(leds.scroll_lock ? PSTR("SCRLK") : PSTR("     "), false);
}

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    return oled_init_user(OLED_ROTATION_270);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        render_status();
    }
    return false;
}

#endif
