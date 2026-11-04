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

// Chordal Hold: opposite-hands rule for Ent*, but NAV (Caps*) may be held
// together with keys on its own (left) hand: F1-F6, RGB keys, BOOT.
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
  if (tap_hold_keycode == LT(_NAV, KC_CAPS)) {
    return true;
  }
  return get_chordal_hold_default(tap_hold_record, other_record);
}

#ifdef RGB_MATRIX_ENABLE
// Indicators drawn on top of any effect:
//  - Caps Lock: Caps key white
//  - NAV layer: arrow keys white, media keys cyan
static void light_key(uint8_t row, uint8_t col, uint8_t r, uint8_t g, uint8_t b, uint8_t led_min, uint8_t led_max) {
  uint8_t i = g_led_config.matrix_co[row][col];
  if (i != NO_LED && i >= led_min && i < led_max) {
    rgb_matrix_set_color(i, r, g, b);
  }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  const uint8_t v = rgb_matrix_get_val();
  if (host_keyboard_led_state().caps_lock) {
    light_key(2, 0, v, v, v, led_min, led_max);
  }
  if (get_highest_layer(layer_state) == _NAV) {
    // right half, matrix rows 5-9 (col 0 = outer edge)
    static const uint8_t arrows[][2] = {
      {6, 3},                  // Up    (I)
      {7, 4}, {7, 3}, {7, 2},  // Left, Down, Right (J K L)
      {9, 3}, {9, 2}, {9, 1}, {9, 0},  // Left, Down, Up, Right (bottom row)
    };
    for (uint8_t k = 0; k < ARRAY_SIZE(arrows); k++) {
      light_key(arrows[k][0], arrows[k][1], v, v, v, led_min, led_max);
    }
    static const uint8_t media[][2] = {{8, 2}, {8, 1}, {8, 0}};  // Play, Prev, Next
    for (uint8_t k = 0; k < ARRAY_SIZE(media); k++) {
      light_key(media[k][0], media[k][1], 0, v, v, led_min, led_max);
    }
  }
  return false;
}
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
