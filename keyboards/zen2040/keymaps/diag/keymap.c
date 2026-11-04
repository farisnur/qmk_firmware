// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H


// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _NAV
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.              .-----------------------------------------.
   * | GESC |   1  |   2  |   3  |   4  |   5  |              |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------|              |------+------+------+------+------+------|
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |              |   Y  |   U  |   I  |   O  |   P  |   \  |
   * |------+------+------+------+------+------|              |------+------+------+------+------+------|
   * |CAPS(NAV)|A  |   S  |   D  |   F  |   G  |              |   H  |   J  |   K  |   L  |   ;  |  "   |
   * |------+------+------+------+------+------+------..------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |RGBPrv||RGBNxt|   N  |   M  |   ,  |   .  |   /  |Enter |
   * |------+------+------+------+------+------+------||------+------+------+------+------+------+------|
   * | Ctrl | GUI  | Alt  |RGBTOG| NAV  | Bksp |Delete||Enter |Space |  NAV |   -  |   =  | PGUP | PGDN |
   * `----------------------------------+-------------''------------------------------------------------'
   *                                    | Bksp |Delete||Enter |Space |
   *                                    '-------------''-------------'
   */
  [_QWERTY] = LAYOUT(
    QK_GESC,          KC_1,    KC_2,    KC_3,    KC_4,     KC_5,                       KC_6,     KC_7,     KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,                       KC_Y,     KC_U,     KC_I,    KC_O,    KC_P,    KC_BSLS,
    LT(_NAV, KC_CAPS),KC_A,    KC_S,    KC_D,    KC_F,     KC_G,                       KC_H,     KC_J,     KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,     KC_B,    UG_PREV, UG_NEXT,  KC_N,     KC_M,     KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENT),
    KC_LCTL,          KC_LALT, KC_LGUI, UG_TOGG, MO(_NAV), KC_BSPC,  KC_DEL,  KC_ENT,   KC_SPACE, MO(_NAV), KC_MINS, KC_EQL,  KC_RGUI, KC_RALT,
                                                           KC_BSPC,  KC_DEL,  KC_ENT,   KC_SPACE
  ),

  /* NAV
   * ,-----------------------------------------.              .-----------------------------------------.
   * | GESC |   1  |   2  |   3  |   4  |   5  |              |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------|              |------+------+------+------+------+------|
   * | Tab  |   Q  |   W  |   E  |   R  |   T  |              |   Y  |   U  |   I  |   O  |   P  |   \  |
   * |------+------+------+------+------+------|              |------+------+------+------+------+------|
   * |CAPS(NAV)|A  |   S  |   D  |   F  |   G  |              |   H  |   J  |   K  |   L  |   ;  |  "   |
   * |------+------+------+------+------+------+------..------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |      ||      |   N  |   M  |   ,  |   .  |   /  |Enter |
   * |------+------+------+------+------+------+------||------+------+------+------+------+------+------|
   * | Ctrl | GUI  | Alt  |RGBTOG| NAV  |Space |Delete||Enter |Space |  NAV |   -  |   =  | PGUP | PGDN |
   * `----------------------------------+-------------''------------------------------------------------'
   *                                    |Space |Delete||Enter |Space |
   *                                    '-------------''-------------'
   */
  [_NAV] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                     KC_F7,   KC_F8,    KC_F9,   KC_F10,  KC_F11,  KC_F12,
    UG_NEXT, UG_SATU, UG_VALU, UG_SATD, QK_BOOT, KC_LBRC,                   KC_RBRC, KC_PGUP,  KC_UP,   KC_PGDN, KC_INS,  KC_HOME,
    _______, UG_HUED, UG_VALD, UG_HUEU, _______, _______,                   _______, KC_LEFT,  KC_DOWN, KC_RGHT, KC_DEL,  RSFT(KC_ENT),
    UG_VALU, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, KC_MPLY, KC_MPRV, KC_MNXT,
    UG_VALD, KC_LGUI, KC_LALT, UG_NEXT, _______, _______, _______, _______, _______, _______,  KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
                                                 _______, _______, _______, _______
  ),

};

#if defined(ENCODER_MAP_ENABLE)
// [layer] = { left encoder, right encoder }
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_QWERTY] = { ENCODER_CCW_CW(MS_WHLU, MS_WHLD), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_NAV]    = { ENCODER_CCW_CW(UG_HUED, UG_HUEU), ENCODER_CCW_CW(UG_SATD, UG_SATU) },
};
#endif

#ifdef OLED_ENABLE
const char* layer_name_user(uint32_t layer) {
  switch (layer) {
    case _QWERTY:
      return PSTR("QWRTY");
    case _NAV:
      return PSTR("NAV\n");
    default:
      return PSTR("UNDEF");
  }
}
#endif

// ---- I2C / OLED diagnostics ----
#include "i2c_master.h"
#include "print.h"
#include "timer.h"

// software I2C on arbitrary pins (open-drain emulation)
#include "wait.h"
static pin_t SDA_, SCL_;
static void hi(pin_t p) { gpio_set_pin_input_high(p); }
static void lo(pin_t p) { gpio_set_pin_output(p); gpio_write_pin_low(p); }
#define D() wait_us(5)
static void start(void) { hi(SDA_); hi(SCL_); D(); lo(SDA_); D(); lo(SCL_); D(); }
static void stop(void)  { lo(SDA_); D(); hi(SCL_); D(); hi(SDA_); D(); }
static bool wr(uint8_t b) {
    for (int i = 7; i >= 0; i--) { if (b & (1 << i)) hi(SDA_); else lo(SDA_); D(); hi(SCL_); D(); lo(SCL_); }
    hi(SDA_); D(); hi(SCL_); D(); bool ack = !gpio_read_pin(SDA_); lo(SCL_); D();
    return ack;
}
static void swscan(const char *name, pin_t sda, pin_t scl) {
    SDA_ = sda; SCL_ = scl;
    uprintf("[diag] sw %s:", name);
    int n = 0;
    for (uint8_t a = 0x08; a < 0x78; a++) { start(); if (wr(a << 1)) { uprintf(" 0x%02X", a); n++; } stop(); }
    uprintf(n ? "\n" : " NONE\n");
}

void housekeeping_task_user(void) {
    static uint32_t last = 0;
    if (timer_elapsed32(last) < 3000) return;
    last = timer_read32();
    swscan("SDA=GP2 SCL=GP3", GP2, GP3);
    swscan("SDA=GP3 SCL=GP2", GP3, GP2);
}
