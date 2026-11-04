// Copyright 2024 medklef
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#define SPLIT_WPM_ENABLE
#define OLED_UPDATE_INTERVAL 50
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_ZEN_AURORA

// Tap-hold tuning (Caps* = LT(NAV, Caps), Ent* = RSFT_T(Enter)).
// Ent* is used as a Shift for left-hand capitals while typing, so:
//  - PERMISSIVE_HOLD: a key pressed and released while Ent*/Caps* is held is
//    always the hold (Shift / NAV), even inside TAPPING_TERM.
//  - CHORDAL_HOLD: Ent* + a right-hand key before TAPPING_TERM is a roll -> tap
//    (Enter). Caps* is exempted in keymap.c so NAV works with left-hand keys.
//  - QUICK_TAP_TERM: tap-then-hold within 120 ms repeats the tap (Enter);
//    later it is the hold.
//  - SPECULATIVE_HOLD: the Shift of Ent* is applied on key-down (Shift+click).
// No FLOW_TAP_TERM: it would turn "space, Ent*+letter" into Enter.
#define PERMISSIVE_HOLD
#define CHORDAL_HOLD
#define QUICK_TAP_TERM 120
#define SPECULATIVE_HOLD
