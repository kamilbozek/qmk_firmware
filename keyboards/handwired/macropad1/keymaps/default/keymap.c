#include QMK_KEYBOARD_H
#include <stdio.h>

enum layers {
    _IDLE = 0,
    _FOCUS,
    _RESULT_SUCCESS,
    _RESULT_FAILURE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ START │ CHANGE_TIME │ 3 │
     * └───┴───┴───┘
     */
    [_IDLE] = LAYOUT_ortho_1x3(
        TO(_FOCUS),   TO(_RESULT_SUCCESS),   KC_3
    ),
    [_FOCUS] = LAYOUT_ortho_1x3(
        TO(_RESULT_FAILURE),   TO(_RESULT_FAILURE),   KC_3
    ),
    [_RESULT_SUCCESS] = LAYOUT_ortho_1x3(
        TO(_IDLE),   TO(_IDLE),   KC_3
    ),
    [_RESULT_FAILURE] = LAYOUT_ortho_1x3(
        TO(_IDLE),   TO(_IDLE),   KC_3
    ),
};


#ifdef OLED_ENABLE

# define ANIM_FRAME_DURATION 200
# define ANIM_SIZE 525

# define IDLE_FRAMES 5
# define IDLE_SPEED 10

# define TAP_FRAMES 2
# define TAP_SPEED 30

char wpm_str[10];
uint32_t timer = 0;
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;

# define TIMER_SET_STEP_SECONDS 10

# define MAX_TARGET_SECONDS 40

# define RESULT_SCREEN_DURATION 5

uint32_t target_duration = 0;


static void render_anim(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _IDLE:
                oled_write_P(PSTR("IDLE\n"), false);
                break;
            case _FOCUS:
                oled_write_P(PSTR("FOCUS\n"), false);
                break;
            case _RESULT_FAILURE:
                oled_write_P(PSTR("FAILURE\n"), false);
                break;
            case _RESULT_SUCCESS:
                oled_write_P(PSTR("SUCCESS\n"), false);
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