#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     * ┌───┬───┬───┐
     * │ 7 │ 8 │ 9 │
     * ├───┼───┼───┤
     * │ 4 │ 5 │ 6 │
     * ├───┼───┼───┤
     * │ 1 │ 2 │ 3 │
     * └───┴───┴───┘
     */
    [0] = LAYOUT_ortho_3x3(
        LGUI(KC_TAB),   LCTL(KC_UP),   LGUI(KC_TAB),
        LCTL(KC_LEFT),   LCTL(KC_DOWN),   LCTL(KC_RIGHT),
        LCA(KC_LEFT),   LCTL(LGUI(KC_F)),   LCA(KC_RIGHT)
    )
};
