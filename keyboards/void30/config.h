#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID 0xF055
#define PRODUCT_ID 0x3532
#define DEVICE_VER 0x0001
#define MANUFACTURER medklef
#define PRODUCT void30keyboard
#define DESCRIPTION 30 % ortholinearkeyboard


/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 10

/* Keyboard Matrix Assignments
 *
 *      1   2   3   4   5   6   7   8   9   10
 */
#define MATRIX_ROW_PINS \
    {                   \
        B4, B3, A15     \
    }
#define MATRIX_COL_PINS                        \
    {                                          \
        B1, B0, A7, A6, A5, A4, A3, A2, A1, A0 \
    }
#define UNUSED_PINS

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* RGB */
#define RGB_DI_PIN B6
#define RGBLED_NUM 30
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#define RGBLIGHT_LIMIT_VAL 255 /* The maximum brightness level */
#define RGBLIGHT_SLEEP         /* If defined, the RGB lighting will be switched off when the host goes to sleep */
// /*== all animations enable ==*/
#define RGBLIGHT_ANIMATIONS
// /*== or choose animations ==*/

#define BOOTMAGIC_LITE_ROW 0
#define BOOTMAGIC_LITE_COLUMN 0

/* disable these deprecated features by default */
#ifndef LINK_TIME_OPTIMIZATION_ENABLE
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#endif

/* Force NKRO */
//#define FORCE_NKRO

