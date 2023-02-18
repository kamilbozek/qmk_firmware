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

# define MAX_TARGET_SECONDS 40000

# define RESULT_SCREEN_DURATION 5000

uint32_t timer = 0;

uint32_t target_duration = 10000;

uint8_t current_state = _IDLE;

uint8_t success_count = 0;

char target_duration_str[5];

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
                }
                break;
            case _FOCUS:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("foucs: go to failure\n");
                    timer = timer_read32();
                    current_state = _RESULT_FAILURE;
                }
                break;
            case _RESULT_FAILURE:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("failure: go to idle\n");
                    current_state = _IDLE;
                }
                break;
            case _RESULT_SUCCESS:
                if (keycode == KC_1 || keycode == KC_2) {
                    uprintf("success: go to idle\n");
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




static void render_anim(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("State: "), false);
    switch (current_state) {
        case _IDLE:
            oled_write_P(PSTR("IDLE\n"), false);
            sprintf(target_duration_str, "%lu", target_duration);
            oled_write_P(PSTR("Target: "), false);
            oled_write(target_duration_str, false);
            break;
        case _FOCUS:
                if (timer_elapsed32(timer) > target_duration) {
                    success_count++;
                    uprintf("focus: go to success on timeout, count=%u \n", success_count);
                    timer = timer_read32();
                    current_state = _RESULT_SUCCESS;
                } else {
                    oled_write_P(PSTR("FOCUS\n"), false);
                }
                break;
            case _RESULT_FAILURE:
                if (timer_elapsed32(timer) > RESULT_SCREEN_DURATION) {
                    uprintf("failure: go to idle on timeout\n");
                    timer = timer_read32();
                    current_state = _IDLE;
                } else {
                    oled_write_P(PSTR("FAILURE\n"), false);
                }
                break;
            case _RESULT_SUCCESS:
                if (timer_elapsed32(timer) > RESULT_SCREEN_DURATION) {
                    uprintf("success: go to idle on timeout\n");
                    timer = timer_read32();
                    current_state = _IDLE;
                } else {
                    oled_write_P(PSTR("SUCCESS\n"), false);
                }
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
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

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    render_anim();
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