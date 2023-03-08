#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _NUM,
    _SYM,
    _NAV,
    _MEDIA
};

// shift functions

const key_override_t capsword_key_override = ko_make_basic(MOD_MASK_SHIFT, CW_TOGG, KC_CAPS);

const key_override_t **key_overrides = (const key_override_t *[]){
    &capsword_key_override,
    NULL
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_kamilbozek(
    KC_Q,              KC_W,              KC_E,              KC_R,              KC_T,              KC_Y,              KC_U,              KC_I,              KC_O,              KC_P,              \
    LGUI_T(KC_A),      LALT_T(KC_S),      LCTL_T(KC_D),      LSFT_T(KC_F),      KC_G,              KC_H,              LSFT_T(KC_J),      LCTL_T(KC_K),      LALT_T(KC_L),      LGUI_T(KC_QUOT),   \
    KC_Z,              KC_X,              KC_C,              KC_V,              KC_B,              KC_N,              KC_M,              KC_COMM,           KC_DOT,            KC_SLSH,\
                                                             LT(_MEDIA,KC_TAB), LT(_NAV,KC_SPC),   LT(_SYM,KC_ENT),   LT(_NUM,KC_BSPC)
    ),

    [_NUM] = LAYOUT_kamilbozek(
    KC_SLSH,           KC_7,              KC_8,              KC_9,              KC_ASTR,           U_NA,              U_NA,              U_NA,              U_NA,              U_NA,     \
    KC_MINS,           KC_4,              KC_5,              KC_6,              KC_PLUS,           U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
    KC_PERC,           KC_1,              KC_2,              KC_3,              KC_PERC,           U_NA,              U_NA,              U_NA,              KC_ALGR,           U_NA,              \
                                                             KC_0,              KC_DOT,            U_NA,              U_NA
    ),

// ` < > " & $ @ [ ] %
// : - + = ; | ! ( ) ?
// ~ / * # \ x . { } ^
//       _ -
    [_SYM] = LAYOUT_kamilbozek(
    KC_GRV,            KC_LABK,           KC_RABK,           KC_DQUO,           KC_AMPR,           KC_DLR,            KC_AT,             KC_LBRC,           KC_RBRC,           KC_PERC,     \
    KC_COLN,           KC_MINS,           KC_PLUS,           KC_EQL,            KC_SCLN,           KC_PIPE,           KC_EXLM,           KC_LPRN,           KC_RPRN,           KC_QUES,           \
    KC_TILD,           KC_SLASH,          KC_ASTR,           KC_HASH,           KC_BSLS,           U_NA,              KC_DOT,            KC_LCBR,           KC_RCBR,           KC_CIRC,              \
                                                             KC_UNDS,           KC_MINS,           U_NA,              U_NA
    ),

    [_NAV] = LAYOUT_kamilbozek(
    U_NA,              U_NA,              U_NA,              U_NA,              U_NA,              U_RDO,             U_PST,             U_CPY,             U_CUT,             U_UND,             \
    KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              KC_LEFT,           KC_DOWN,           KC_UP,             KC_RGHT,           CW_TOGG,           \
    U_NA,              KC_ALGR,           U_NA,              U_NA,              U_NA,              KC_HOME,           KC_PGDN,           KC_PGUP,           KC_END,            KC_ESC,            \
                                                             U_NA,              U_NA,              KC_ENT,            KC_DEL
    ),

    [_MEDIA] = LAYOUT_kamilbozek(
    U_NA,              U_NA,              U_NA,              U_NA,              U_NA,              RGB_MOD,           RGB_HUI,           RGB_SAI,           RGB_VAI,           RGB_TOG,           \
    KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              U_NA,              KC_VOLD,           KC_VOLU,           U_NA,              U_NU,              \
    U_NA,              KC_ALGR,           U_NA,              U_NA,              U_NA,              U_NU,              U_NU,              U_NU,              U_NU,              U_NU,           \
                                                             U_NA,              U_NA,              U_NU,              U_NU
    )
};
