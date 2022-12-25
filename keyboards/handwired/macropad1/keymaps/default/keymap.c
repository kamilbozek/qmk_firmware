#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ 1 │ 2 │ 3 │
     * └───┴───┴───┘
     */
    [0] = LAYOUT_ortho_1x3(
        KC_1,   KC_2,   KC_3
    )
};
