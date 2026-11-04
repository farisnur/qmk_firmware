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

  /* QWERTY  (full key map: ../../MAPA_KLAWISZY.md)
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * |Esc ` |  1   |  2   |  3   |  4   |  5   |                   |  6   |  7   |  8   |  9   |  0   | Bksp |
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * | Tab  |  Q   |  W   |  E   |  R   |  T   |                   |  Y   |  U   |  I   |  O   |  P   |  \   |
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * |Caps* |  A   |  S   |  D   |  F   |  G   |                   |  H   |  J   |  K   |  L   |  ;   |  '   |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   * |Shift |  Z   |  X   |  C   |  V   |  B   | RGB< |     | RGB> |  N   |  M   |  ,   |  .   |  /   | Ent* |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   * | Ctrl | Alt  | Cmd  |RGBon | NAV  |                                 | NAV  |  -   |  =   | Cmd  | Alt  |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   *                                    | Bksp | Del  |     |Enter |Space |
   *                                    +------+------+     +------+------+
   *                                    | Bksp | Del  |     |Enter |Space |
   *                                    +------+------+     +------+------+
   *
   * Caps* tap = Caps Lock, hold = NAV    Ent* tap = Enter, hold = Right Shift
   * Esc ` = Grave Escape (Shift+Esc = ~, Cmd+Esc = `)
   */
  [_QWERTY] = LAYOUT(
    QK_GESC,          KC_1,    KC_2,    KC_3,    KC_4,     KC_5,                       KC_6,     KC_7,     KC_8,    KC_9,    KC_0,    KC_BSPC,
    KC_TAB,           KC_Q,    KC_W,    KC_E,    KC_R,     KC_T,                       KC_Y,     KC_U,     KC_I,    KC_O,    KC_P,    KC_BSLS,
    LT(_NAV, KC_CAPS),KC_A,    KC_S,    KC_D,    KC_F,     KC_G,                       KC_H,     KC_J,     KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,     KC_B,    UG_PREV, UG_NEXT,  KC_N,     KC_M,     KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENT),
    KC_LCTL,          KC_LALT, KC_LGUI, UG_TOGG, MO(_NAV), KC_BSPC,  KC_DEL,  KC_ENT,   KC_SPACE, MO(_NAV), KC_MINS, KC_EQL,  KC_RGUI, KC_RALT,
                                                           KC_BSPC,  KC_DEL,  KC_ENT,   KC_SPACE
  ),

  /* NAV  (hold Caps* or NAV)
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                   |  F7  |  F8  |  F9  | F10  | F11  | F12  |
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * | RGB> | Sat+ | Jas+ | Sat- | BOOT |  [   |                   |  ]   | PgUp |  Up  | PgDn | Ins  | Home |
   * +------+------+------+------+------+------+                   +------+------+------+------+------+------+
   * |(trz) | Hue- | Jas- | Hue+ |      |      |                   |      | Left | Down |Right | Del  |S+Ent |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   * | Jas+ |      |      |      |      |      |      |     |      |      |      |      | Play | Prev | Next |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   * | Jas- | Cmd  | Alt  | RGB> |(trz) |                                 |(trz) | Left | Down |  Up  |Right |
   * +------+------+------+------+------+------+------+     +------+------+------+------+------+------+------+
   *                                    |      |      |     |      |      |
   *                                    +------+------+     +------+------+
   *                                    |      |      |     |      |      |
   *                                    +------+------+     +------+------+
   *
   * (trz) = held NAV key, blank = transparent    Jas = brightness, Sat = saturation, Hue = hue
   * RGB> = next RGB effect    S+Ent = Shift+Enter    BOOT = bootloader
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
#include "hamster.h"

// USB half: keyboard-level status screen; other half: WPM hamster.
bool oled_task_user(void) {
  if (is_keyboard_master()) {
    return true;
  }
  render_hamster();
  return false;
}

// Exactly 5 characters (one OLED line in portrait).
const char* layer_name_user(uint8_t layer) {
  switch (layer) {
    case _QWERTY:
      return PSTR("QWRTY");
    case _NAV:
      return PSTR(" NAV ");
    default:
      return PSTR("?????");
  }
}
#endif
