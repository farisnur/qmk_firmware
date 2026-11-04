// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Keyboard-level OLED: both displays run in portrait (OLED_ROTATION_270,
// 32 x 128 px = 5 chars x 16 lines). The USB (master) half shows the status
// screen below; what the other half shows is up to the keymap.

#include "quantum.h"

#ifdef OLED_ENABLE

// Layer label, exactly 5 characters so it fills one display line.
__attribute__((weak)) const char *layer_name_user(uint8_t layer) {
    return PSTR("?????");
}

// Host OS icon: 2 x 2 glyphs from glcdfont.c
static void render_os_icon(void) {
    static const char PROGMEM icons[4][2][3] = {
        {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}}, // Apple
        {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}}, // Windows
        {{0x99, 0x9a, 0}, {0xb9, 0xba, 0}}, // Linux
        {{0x9b, 0x9c, 0}, {0xbb, 0xbc, 0}}, // other
    };
    uint8_t os = 3;
#    ifdef OS_DETECTION_ENABLE
    switch (detected_host_os()) {
        case OS_MACOS:
        case OS_IOS:
            os = 0;
            break;
        case OS_WINDOWS:
            os = 1;
            break;
        case OS_LINUX:
            os = 2;
            break;
        default:
            break;
    }
#    endif
    oled_set_cursor(0, 0);
    oled_write_P(icons[os][0], false);
    oled_set_cursor(0, 1);
    oled_write_P(icons[os][1], false);
}

__attribute__((weak)) void render_status(void) {
    render_os_icon();

    oled_set_cursor(0, 3);
    oled_write_P(PSTR("Layer"), false);
    oled_set_cursor(0, 4);
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
