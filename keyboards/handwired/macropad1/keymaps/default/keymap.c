#include QMK_KEYBOARD_H
#include <stdio.h>
#include "print.h"

enum states {
    _IDLE = 0,
    _FOCUS,
    _RESULT_SUCCESS,
    _RESULT_FAILURE
};

// TODO: custom keycodes, save state eeprom, don't turn off oled

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ START │ CHANGE_TIME │ 3 │
     * └───┴───┴───┘
     */
    [0] = LAYOUT_ortho_1x3(
        KC_1,   KC_2,   KC_3
    )
};

# define TIMER_SET_STEP_MILLIS 10000

# define TARGET_MAX_MILLIS 40000

# define RESULT_SCREEN_DURATION 5000

# define TARGET_MIN_MILLIS 10000

uint32_t timer = 0;

uint32_t target_duration = TARGET_MIN_MILLIS;

uint8_t current_state = _IDLE;

uint8_t success_count = 0;

char success_count_str[8];
char target_duration_str[8];
char elapsed_str[8];

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
    
    if (record->event.pressed) {
        switch (current_state) {
            case _IDLE:
                if (keycode == KC_1) {
                    uprintf("idle: go to focus\n");
                    timer = timer_read32();
                    current_state = _FOCUS;
                } else if (keycode == KC_2) {
                    uprintf("idle: change duration from: %lu to %lu\n", target_duration, target_duration + TIMER_SET_STEP_MILLIS);
                    target_duration += TIMER_SET_STEP_MILLIS;
                    if (target_duration > TARGET_MAX_MILLIS) {
                        target_duration = TARGET_MIN_MILLIS;
                    }
                }
                break;
            case _FOCUS:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("foucs: go to failure\n");
                    timer = timer_read32();
                    oled_clear();
                    current_state = _RESULT_FAILURE;
                }
                break;
            case _RESULT_FAILURE:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("failure: go to idle\n");
                    oled_clear();
                    current_state = _IDLE;
                }
                break;
            case _RESULT_SUCCESS:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("success: go to idle\n");
                    oled_clear();
                    current_state = _IDLE;
                }
                break;
        }
    }

    return true;
};


#ifdef OLED_ENABLE

# define ANIM_FRAME_DURATION 200
# define ANIM_SIZE 525

# define IDLE_FRAMES 5
# define IDLE_SPEED 10

# define TAP_FRAMES 2
# define TAP_SPEED 30

char wpm_str[10];
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;




static const char PROGMEM frame0[]= {
// 'sleep-1', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x80, 0x80, 0xc0, 0x40, 0x40, 0x60, 0x20, 0x20, 0x30, 0x10, 0x10, 0x18, 0x08, 
0x08, 0x0c, 0x0c, 0x04, 0x04, 0x04, 0x04, 0x84, 0x04, 0x06, 0x06, 0x02, 0x02, 0x82, 0x82, 0x82, 
0x82, 0xc2, 0xc2, 0xc2, 0xc3, 0xc0, 0xe4, 0xff, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 
0x60, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x60, 0xe0, 0x30, 0x20, 0x20, 0x60, 0x60, 0x30, 0x10, 0x18, 0x0c, 0x04, 0x06, 0x02, 0x02, 
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x80, 0x00, 0x80, 0xc0, 0xc7, 
0x78, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x0c, 0x07, 0x07, 0x02, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x01, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x02, 0x06, 0x0c, 0x0c, 0x18, 
0x30, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x1e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 
0x20, 0x20, 0x60, 0x40, 0x80, 0x80, 0xc6, 0x7c, 0x78, 0x78, 0x0c, 0x1f, 0x3f, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1c, 0x38, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0xe0, 0x70, 0x1c, 0x0f, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0xf0, 0xe0, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x3c, 0x38, 
0x3c, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x78, 0x60, 0x70, 0x38, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0xfe, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1e, 0xf0, 0x80, 0x00, 0x00, 0x06, 
0x0f, 0x19, 0x18, 0x08, 0x08, 0x0c, 0x38, 0x7c, 0xcc, 0xc0, 0xe0, 0x60, 0x00, 0x01, 0x00, 0x00, 
0x18, 0x1c, 0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0e, 0x38, 0x60, 
0xc0, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x06, 0x03, 0x03, 0x03, 0x0e, 0xfe, 0x3e, 0x06, 0x00, 0x00, 
0x00, 0x00, 0x0e, 0x1e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0xc0, 0xf0, 0xfc, 0xde, 0x8e, 0x8e, 0xce, 0xfe, 0x3f, 0x07, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x7e, 0x64, 0x60, 0x30, 0x18, 0x08, 0x08, 0x0c, 0x0f, 0x0f, 0x0c, 0x0c, 0x0c, 0x0c, 
0x0c, 0x0c, 0x0c, 0x1e, 0x0e, 0x06, 0x06, 0x1f, 0x0e, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x0e, 
0x0e, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x0c, 0x0e, 0x0e, 0x04, 0x04, 0x0c, 0x1e, 0x1f, 
0x3f, 0x1f, 0x18, 0x18, 0x0d, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x1a, 0x00, 0x08, 0x0e, 0x00, 0x1e, 0x0a, 0x06, 0x00, 0x0c, 
0x0c, 0x0e, 0x06, 0x00, 0x1e, 0x00, 0x06, 0x0c, 0x1e, 0x04, 0x12, 0x1a, 0x08, 0x00, 0x00, 0x00
};

// 'sleep-2', 128x64px
static const char PROGMEM frame1[]= {
// 'sleep-2', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x80, 0xc0, 0x40, 0x40, 0x60, 0x20, 0x20, 0x30, 0x10, 0x10, 0x10, 0x18, 0x08, 
0x08, 0x08, 0x0c, 0x0c, 0x04, 0x04, 0x04, 0x84, 0x84, 0x04, 0x86, 0x86, 0x82, 0xc2, 0xc2, 0xc2, 
0x42, 0x62, 0x62, 0x62, 0x63, 0xe1, 0xe0, 0x7f, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 
0x60, 0x60, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x40, 0xe0, 0x20, 0x20, 0x60, 0x40, 0x60, 0x30, 0x10, 0x18, 0x08, 0x04, 0x04, 0x06, 0x02, 
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x80, 0xc0, 0xc0, 0x67, 
0x3c, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x02, 0x06, 0x0c, 0x0c, 
0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x1e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 
0x20, 0x20, 0x60, 0x40, 0x80, 0x80, 0xce, 0x78, 0x78, 0xfc, 0x0f, 0x1f, 0x3f, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0e, 0x1c, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0xe0, 0x38, 0x1c, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0xe0, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1e, 
0x1e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x70, 0x38, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x78, 0xc0, 0x00, 0x00, 0x0c, 
0x0f, 0x19, 0x18, 0x18, 0x08, 0x18, 0x38, 0x7c, 0xcc, 0xc0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x1c, 0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x06, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0x18, 0x60, 
0xc0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x03, 0x03, 0x07, 0x0e, 0xfc, 0x7e, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x1e, 0x1e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x80, 0xc0, 0xf0, 0xfc, 0xfe, 0xce, 0x8e, 0xde, 0x7e, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x7e, 0x4c, 0x60, 0x30, 0x18, 0x08, 0x08, 0x0c, 0x0e, 0x0f, 0x08, 0x08, 0x08, 0x0c, 
0x0c, 0x0c, 0x0c, 0x1e, 0x0f, 0x06, 0x06, 0x1f, 0x1e, 0x06, 0x06, 0x04, 0x06, 0x06, 0x07, 0x0e, 
0x0e, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x08, 0x0e, 0x0e, 0x06, 0x04, 0x0e, 0x1e, 0x3f, 
0x3f, 0x1d, 0x18, 0x18, 0x0d, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x1a, 0x00, 0x08, 0x0e, 0x00, 0x1e, 0x0a, 0x06, 0x00, 0x0c, 
0x0c, 0x0e, 0x06, 0x00, 0x1e, 0x00, 0x06, 0x0c, 0x1e, 0x04, 0x12, 0x1a, 0x08, 0x00, 0x00, 0x00
};

// 'sleep-3', 128x64px
static const char PROGMEM frame2[]= {
// 'sleep-3', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x80, 0xc0, 0x40, 0x40, 0x60, 0x20, 0x20, 0x30, 0x10, 0x10, 0x10, 0x18, 0x08, 
0x08, 0x08, 0x0c, 0x0c, 0x04, 0x04, 0x04, 0xc4, 0x84, 0x84, 0x86, 0x86, 0xc6, 0xc2, 0x42, 0x42, 
0x62, 0x62, 0x62, 0x62, 0x61, 0x70, 0xf4, 0x7f, 0x70, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 
0x30, 0x60, 0xe0, 0xe0, 0xe0, 0xe0, 0x40, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x40, 0xe0, 0x20, 0x20, 0x20, 0x60, 0x60, 0x30, 0x10, 0x08, 0x0c, 0x04, 0x04, 0x02, 0x02, 
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x67, 
0x38, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x02, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x02, 0x06, 0x0c, 
0x18, 0x30, 0x70, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x1e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 
0x20, 0x60, 0x40, 0xc0, 0x80, 0x8c, 0xd8, 0x70, 0x70, 0xd9, 0x0f, 0x1f, 0x33, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x1c, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0xe0, 0x78, 0x1c, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x0e, 
0x0e, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x3c, 0x30, 0x3c, 0x1c, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7f, 0xf0, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0x70, 0xc0, 0x00, 0x00, 0x04, 
0x0f, 0x09, 0x08, 0x08, 0x08, 0x08, 0x38, 0x7c, 0xcc, 0xc0, 0xe0, 0x60, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x1c, 0x1e, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0x18, 0x60, 
0xc0, 0x80, 0x00, 0x00, 0x00, 0x0c, 0x06, 0x03, 0x03, 0x07, 0x0e, 0xfe, 0x3e, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x0e, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x80, 0xe0, 0xf8, 0xfc, 0xce, 0x8e, 0x8e, 0xfe, 0x7e, 0x0f, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x7b, 0x4c, 0x60, 0x30, 0x18, 0x08, 0x08, 0x0c, 0x0f, 0x0f, 0x08, 0x08, 0x0c, 0x0c, 
0x0c, 0x04, 0x0c, 0x1e, 0x0f, 0x06, 0x07, 0x1f, 0x1e, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x0e, 
0x0e, 0x07, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x08, 0x0e, 0x0e, 0x06, 0x04, 0x0e, 0x1e, 0x3f, 
0x3f, 0x3f, 0x18, 0x18, 0x09, 0x0f, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x1a, 0x00, 0x08, 0x0e, 0x00, 0x1e, 0x0a, 0x06, 0x00, 0x0c, 
0x0c, 0x0e, 0x06, 0x00, 0x1e, 0x00, 0x06, 0x0c, 0x1e, 0x04, 0x12, 0x1a, 0x08, 0x00, 0x00, 0x00
};

// 'sleep-4', 128x64px
static const char PROGMEM frame3[]= {
// 'sleep-4', 128x64px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x80, 0x80, 0xc0, 0x40, 0x40, 0x60, 0x20, 0x20, 0x30, 0x10, 0x10, 0x10, 0x18, 0x08, 
0x08, 0x08, 0x0c, 0x0c, 0x04, 0x04, 0x04, 0x84, 0x84, 0x04, 0x86, 0x86, 0x82, 0xc2, 0xc2, 0xc2, 
0x42, 0x62, 0x62, 0x62, 0x63, 0xe1, 0xe0, 0x7f, 0x70, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60, 
0x60, 0x60, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x40, 0xe0, 0x20, 0x20, 0x60, 0x40, 0x60, 0x30, 0x10, 0x18, 0x08, 0x04, 0x04, 0x06, 0x02, 
0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x80, 0xc0, 0xc0, 0x67, 
0x3c, 0x18, 0x18, 0x0c, 0x0c, 0x06, 0x06, 0x03, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x02, 0x06, 0x0c, 0x0c, 
0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x01, 0x01, 0x01, 0x03, 0x1e, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 
0x20, 0x20, 0x60, 0x40, 0x80, 0x80, 0xce, 0x78, 0x78, 0xfc, 0x0f, 0x1f, 0x3f, 0x01, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0e, 0x1c, 0x70, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0xe0, 0x38, 0x1c, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x20, 0x70, 0xe0, 0x80, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1e, 
0x1e, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3c, 0x70, 0x38, 0x18, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0f, 0x78, 0xc0, 0x00, 0x00, 0x0c, 
0x0f, 0x19, 0x18, 0x18, 0x08, 0x18, 0x38, 0x7c, 0xcc, 0xc0, 0xe0, 0xe0, 0x00, 0x00, 0x00, 0x00, 
0x18, 0x1c, 0x1e, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x06, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0e, 0x18, 0x60, 
0xc0, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x03, 0x03, 0x07, 0x0e, 0xfc, 0x7e, 0x02, 0x00, 0x00, 
0x00, 0x00, 0x1e, 0x1e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x80, 0x80, 0xc0, 0xf0, 0xfc, 0xfe, 0xce, 0x8e, 0xde, 0x7e, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x01, 0x7e, 0x4c, 0x60, 0x30, 0x18, 0x08, 0x08, 0x0c, 0x0e, 0x0f, 0x08, 0x08, 0x08, 0x0c, 
0x0c, 0x0c, 0x0c, 0x1e, 0x0f, 0x06, 0x06, 0x1f, 0x1e, 0x06, 0x06, 0x04, 0x06, 0x06, 0x07, 0x0e, 
0x0e, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x0e, 0x08, 0x0e, 0x0e, 0x06, 0x04, 0x0e, 0x1e, 0x3f, 
0x3f, 0x1d, 0x18, 0x18, 0x0d, 0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x1e, 0x0c, 0x1a, 0x00, 0x08, 0x0e, 0x00, 0x1e, 0x0a, 0x06, 0x00, 0x0c, 
0x0c, 0x0e, 0x06, 0x00, 0x1e, 0x00, 0x06, 0x0c, 0x1e, 0x04, 0x12, 0x1a, 0x08, 0x00, 0x00, 0x00
};

uint32_t sleepy_timer = 0;
uint8_t frame_number = 0;

#define SLEEPY_FRAME_DURATION 300

static void render_image(void) {
    if (timer_elapsed32(sleepy_timer) > SLEEPY_FRAME_DURATION) {
        oled_set_cursor(0, 0);
        sleepy_timer = timer_read32();
        switch (frame_number) {
            case 0:
                printf("print frame 0 \n");
                // oled_set_cursor(0, 2);
                // oled_write_P(PSTR("frame 0\n"), false);
                oled_write_raw_P(frame0, sizeof(frame0));
                break;
            case 1:
                oled_write_raw_P(frame1, sizeof(frame1));
                // oled_set_cursor(0, 2);
                // oled_write_P(PSTR("frame 1\n"), false);
                printf("print frame 1 \n");
                break;
            case 2:
                oled_write_raw_P(frame2, sizeof(frame2));
                // oled_set_cursor(0, 2);
                // oled_write_P(PSTR("frame 2\n"), false);
                printf("print frame 2 \n");
                break;
            case 3:
                oled_write_raw_P(frame3, sizeof(frame3));
                // oled_set_cursor(0, 2);
                // oled_write_P(PSTR("frame 3\n"), false);
                printf("print frame 3 \n");
                break;
        }
        frame_number = (frame_number + 1) % 4;
        oled_set_cursor(0, 0);
        oled_write_P(PSTR("              Focus!"), false);
    }
}

// static void render_animation(void) {
//     if (timer_elapsed32(timer) > ANIM_FRAME_DURATION) {
//         uint8_t wpm = get_current_wpm();
//         if (wpm < IDLE_SPEED) {
//             // oled_write_P(PSTR("\n\n\nIDLE\n"), false);
//             current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
//             current_tap_frame = 0;
// 			oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
//         } else if (wpm < TAP_SPEED) {
//             // oled_write_P(PSTR("\n\n\nGET READY...\n"), false);
//             current_idle_frame = 0;
//             current_tap_frame = 0;
// 			oled_write_raw_P(prep[0], ANIM_SIZE);
//         } else {
//             // oled_write_P(PSTR("\n\n\nTAP! TAP! TAP!\n"), false);
//             current_idle_frame = 0;
//             current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
// 			oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE);
//         }
//         timer = timer_read32();
//     }
// }

static void render_anim(void) {
    // sprintf(success_count_str, "%u", success_count);
    // oled_set_cursor(0, 4);
    // oled_write_P(PSTR("Successes: "), false);
    // oled_write(success_count_str, false);

    // oled_set_cursor(0, 0);
    // oled_write_P(PSTR("State: "), false);
    switch (current_state) {
        case _IDLE:
            oled_write_P(PSTR("IDLE\n"), false);
            // print in seconds, watch out not to write outside of the array
            sprintf(target_duration_str, "%lu", target_duration / 1000);
            oled_set_cursor(0, 1);
            oled_write_P(PSTR("Target: "), false);
            oled_write(target_duration_str, false);
            break;
        case _FOCUS:
            // oled_write_P(PSTR("FOCUS\n"), false);
            uint32_t elapsed = timer_elapsed32(timer);
            // sprintf(elapsed_str, "%lu", elapsed / 1000);
            // oled_set_cursor(0, 2);
            // oled_write_P(PSTR("Elapsed: "), false);
            // oled_write(elapsed_str, false);
            if (elapsed > target_duration) {
                success_count++;
                uprintf("focus: go to success on timeout, count=%u \n", success_count);
                timer = timer_read32();
                current_state = _RESULT_SUCCESS;
            } else {
                render_image();
            }
            break;
        case _RESULT_FAILURE:
            if (timer_elapsed32(timer) > RESULT_SCREEN_DURATION) {
                uprintf("failure: go to idle on timeout\n");
                timer = timer_read32();
                oled_clear();
                current_state = _IDLE;
            } else {
                oled_write_P(PSTR("FAILURE\n"), false);
            }
            break;
        case _RESULT_SUCCESS:
            if (timer_elapsed32(timer) > RESULT_SCREEN_DURATION) {
                uprintf("success: go to idle on timeout\n");
                timer = timer_read32();
                oled_clear();
                current_state = _IDLE;
            } else {
                oled_write_P(PSTR("SUCCESS\n"), false);
            }
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
        }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    render_anim();
    // render_image();
        // render_animation();  // renders pixelart

        // oled_set_cursor(0, 6);                            // sets cursor to (row, column) using charactar spacing (5 rows on 128x32 screen, anything more will overflow back to the top)
        // sprintf(wpm_str, "WPM:%03d", get_current_wpm());  // edit the string to change wwhat shows up, edit %03d to change how many digits show up
        // oled_write(wpm_str, false);                       // writes wpm on top left corner of string

        // led_t led_state = host_keyboard_led_state();  // caps lock stuff, prints CAPS on new line if caps led is on
        // oled_set_cursor(0, 7);
        // oled_write_P(led_state.caps_lock ? PSTR("CAPS") : PSTR("       "), false);
    return false;
}
#endif