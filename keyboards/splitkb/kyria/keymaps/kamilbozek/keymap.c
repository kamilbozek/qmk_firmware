#include QMK_KEYBOARD_H


enum layers {
    _BASE = 0,
    _NAV,
    _MEDIA,
    _NUM,
    _SYM,
    _FUN,
};


/*
#define LAYOUT( \
    L00, L01, L02, L03, L04, L05,                     R06, R07, R08, R09, R10, R11, \
    L12, L13, L14, L15, L16, L17,                     R18, R19, R20, R21, R22, R23, \
    L24, L25, L26, L27, L28, L29, L30, L31, R32, R33, R34, R35, R36, R37, R38, R39, \
                   L40, L41, L42, L43, L44, R45, R46, R47, R48, R49 \
) \
{ \
    { KC_NO, KC_NO, L05,   L04,   L03,   L02,   L01,   L00   }, \
    { KC_NO, KC_NO, L17,   L16,   L15,   L14,   L13,   L12   }, \
    { L31,   L30,   L29,   L28,   L27,   L26,   L25,   L24   }, \
    { L44,   L43,   L42,   L41,   KC_NO, L40,   KC_NO, KC_NO }, \
    { KC_NO, KC_NO, R06,   R07,   R08,   R09,   R10,   R11   }, \
    { KC_NO, KC_NO, R18,   R19,   R20,   R21,   R22,   R23   }, \
    { R32,   R33,   R34,   R35,   R36,   R37,   R38,   R39   }, \
    { R45,   R46,   R47,   R48,   KC_NO, R49,   KC_NO, KC_NO }, \
}
*/
#define CUSTOM_LAYOUT( \
    L00, L01, L02, L03, L04, L05,                     R06, R07, R08, R09, R10, R11, \
    L12, L13, L14, L15, L16, L17,                     R18, R19, R20, R21, R22, R23, \
    L24, L25, L26, L27, L28, L29,                     R34, R35, R36, R37, R38, R39, \
                             L42, L43, L44, R45, R46, R47 \
) \
{ \
    { KC_NO, KC_NO, L05,   L04,   L03,   L02,   L01,   L00   }, \
    { KC_NO, KC_NO, L17,   L16,   L15,   L14,   L13,   L12   }, \
    { KC_NO, KC_NO, L29,   L28,   L27,   L26,   L25,   L24   }, \
    { L44,   L43,   L42,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
    { KC_NO, KC_NO, R06,   R07,   R08,   R09,   R10,   R11   }, \
    { KC_NO, KC_NO, R18,   R19,   R20,   R21,   R22,   R23   }, \
    { KC_NO, KC_NO, R34,   R35,   R36,   R37,   R38,   R39   }, \
    { R45,   R46,   R47,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
}



// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/*
#define MIRYOKU_ALTERNATIVES_BASE_QWERTY \
KC_Q,              KC_W,              KC_E,              KC_R,              KC_T,              KC_Y,              KC_U,              KC_I,              KC_O,              KC_P,              \
LGUI_T(KC_A),      LALT_T(KC_S),      LCTL_T(KC_D),      LSFT_T(KC_F),      KC_G,              KC_H,              LSFT_T(KC_J),      LCTL_T(KC_K),      LALT_T(KC_L),      LGUI_T(KC_QUOT),   \
LT(U_BUTTON,KC_Z), ALGR_T(KC_X),      KC_C,              KC_V,              KC_B,              KC_N,              KC_M,              KC_COMM,           ALGR_T(KC_DOT),    LT(U_BUTTON,KC_SLSH),\
U_NP,              U_NP,              LT(U_MEDIA,KC_ESC),LT(U_NAV,KC_SPC),  LT(U_MOUSE,KC_TAB),LT(U_SYM,KC_ENT),  LT(U_NUM,KC_BSPC), LT(U_FUN,KC_DEL),  U_NP,              U_NP
*/
    [_BASE] = CUSTOM_LAYOUT(
     KC_GRV  , KC_Q ,  KC_W   ,  KC_E  ,   KC_R ,   KC_T ,                                        KC_Y,   KC_U ,  KC_I ,   KC_O ,  KC_P , KC_BSLS,
     CW_TOGG , LCTL_T(KC_A),  LALT_T(KC_S)   ,  LGUI_T(KC_D)  ,   LSFT_T(KC_F) ,   KC_G ,                                        KC_H,   LSFT_T(KC_J) ,  LGUI_T(KC_K) ,   LALT_T(KC_L) ,LCTL_T(KC_SCLN), KC_QUOT,
     _______ , KC_Z ,  KC_X   ,  KC_C  ,   KC_V ,   KC_B ,  KC_N,   KC_M ,KC_COMM, KC_DOT ,KC_SLSH, _______,
                                LT(_MEDIA,KC_ESC) , KC_SPC, LT(_NAV,KC_TAB),     LT(_SYM,KC_ENT)    , LT(_NUM,KC_BSPC) ,LT(_FUN,KC_DEL)
    ),


/*
#define MIRYOKU_ALTERNATIVES_NAV_VI \
TD(U_TD_BOOT),     TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_NA,              U_RDO,             U_PST,             U_CPY,             U_CUT,             U_UND,             \
KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              KC_LEFT,           KC_DOWN,           KC_UP,             KC_RGHT,           CW_TOGG,           \
U_NA,              KC_ALGR,           TD(U_TD_U_NUM),    TD(U_TD_U_NAV),    U_NA,              KC_HOME,           KC_PGDN,           KC_PGUP,           KC_END,            KC_INS,            \
U_NP,              U_NP,              U_NA,              U_NA,              U_NA,              KC_ENT,            KC_BSPC,           KC_DEL,            U_NP,              U_NP
*/
    [_NAV] = CUSTOM_LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______,   _______,  _______, _______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_CAPS, _______,
      _______, _______, _______, _______, _______, _______,                                     KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_INS, _______,
                                                   _______, _______, _______, KC_ENT, KC_BSPC, KC_DEL
    ),


/*
#define MIRYOKU_ALTERNATIVES_MEDIA_VI \
TD(U_TD_BOOT),     TD(U_TD_U_TAP),    TD(U_TD_U_EXTRA),  TD(U_TD_U_BASE),   U_NA,              RGB_MOD,           RGB_HUI,           RGB_SAI,           RGB_VAI,           RGB_TOG,           \
KC_LGUI,           KC_LALT,           KC_LCTL,           KC_LSFT,           U_NA,              KC_MPRV,           KC_VOLD,           KC_VOLU,           KC_MNXT,           U_NU,              \
U_NA,              KC_ALGR,           TD(U_TD_U_FUN),    TD(U_TD_U_MEDIA),  U_NA,              U_NU,              U_NU,              U_NU,              U_NU,              OU_AUTO,           \
U_NP,              U_NP,              U_NA,              U_NA,              U_NA,              KC_MSTP,           KC_MPLY,           KC_MUTE,           U_NP,              U_NP
*/
    [_MEDIA] = CUSTOM_LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     RGB_MOD, RGB_HUI, RGB_SAI,   RGB_VAI,  RGB_TOG, _______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,                                     KC_MPRV, KC_VOLD, KC_VOLU, KC_MNXT, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
                                                   _______, _______, _______, KC_MSTP, KC_MPLY, KC_MUTE
    ),


/*
#define MIRYOKU_ALTERNATIVES_NUM \
KC_LBRC,           KC_7,              KC_8,              KC_9,              KC_RBRC,           U_NA,              TD(U_TD_U_BASE),   TD(U_TD_U_EXTRA),  TD(U_TD_U_TAP),    TD(U_TD_BOOT),     \
KC_SCLN,           KC_4,              KC_5,              KC_6,              KC_EQL,            U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
KC_GRV,            KC_1,              KC_2,              KC_3,              KC_BSLS,           U_NA,              TD(U_TD_U_NUM),    TD(U_TD_U_NAV),    KC_ALGR,           U_NA,              \
U_NP,              U_NP,              KC_DOT,            KC_0,              KC_MINS,           U_NA,              U_NA,              U_NA,              U_NP,              U_NP
*/
    [_NUM] = CUSTOM_LAYOUT(
      _______, KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC,                                     _______, _______, _______,   _______,  _______, _______,
      _______, KC_SCLN, KC_4, KC_5, KC_6, KC_EQL,                                     _______, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, _______,
      _______, KC_GRV, KC_1, KC_2, KC_3, KC_BSLS,                                     _______, _______, _______, _______, _______, _______,
                                                   KC_DOT, KC_0, KC_MINS, _______, _______, _______
    ),


/*
#define MIRYOKU_ALTERNATIVES_SYM \
KC_LCBR,           KC_AMPR,           KC_ASTR,           KC_LPRN,           KC_RCBR,           U_NA,              TD(U_TD_U_BASE),   TD(U_TD_U_EXTRA),  TD(U_TD_U_TAP),    TD(U_TD_BOOT),     \
KC_COLN,           KC_DLR,            KC_PERC,           KC_CIRC,           KC_PLUS,           U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
KC_TILD,           KC_EXLM,           KC_AT,             KC_HASH,           KC_PIPE,           U_NA,              TD(U_TD_U_SYM),    TD(U_TD_U_MOUSE),  KC_ALGR,           U_NA,              \
U_NP,              U_NP,              KC_LPRN,           KC_RPRN,           KC_UNDS,           U_NA,              U_NA,              U_NA,              U_NP,              U_NP
*/
    [_SYM] = CUSTOM_LAYOUT(
      _______, KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR,                                     _______, _______, _______,   _______,  _______, _______,
      _______, KC_COLN, KC_DLR, KC_PERC, KC_CIRC, KC_PLUS,                                     _______, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, _______,
      _______, KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE,                                     _______, _______, _______, _______, _______, _______,
                                                   KC_LPRN, KC_RPRN, KC_UNDS, _______, _______, _______
    ),


/*
#define MIRYOKU_ALTERNATIVES_FUN \
KC_F12,            KC_F7,             KC_F8,             KC_F9,             KC_PSCR,           U_NA,              TD(U_TD_U_BASE),   TD(U_TD_U_EXTRA),  TD(U_TD_U_TAP),    TD(U_TD_BOOT),     \
KC_F11,            KC_F4,             KC_F5,             KC_F6,             KC_SCRL,           U_NA,              KC_LSFT,           KC_LCTL,           KC_LALT,           KC_LGUI,           \
KC_F10,            KC_F1,             KC_F2,             KC_F3,             KC_PAUS,           U_NA,              TD(U_TD_U_FUN),    TD(U_TD_U_MEDIA),  KC_ALGR,           U_NA,              \
U_NP,              U_NP,              KC_APP,            KC_SPC,            KC_TAB,            U_NA,              U_NA,              U_NA,              U_NP,              U_NP
*/
    [_FUN] = CUSTOM_LAYOUT(
      _______, KC_F12, KC_F7, KC_F8, KC_F9, KC_PSCR,                                     _______, _______, _______,   _______,  _______, _______,
      _______, KC_F11, KC_F4, KC_F5, KC_F6, KC_SCRL,                                     _______, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL, _______,
      _______, KC_F10, KC_F1, KC_F2, KC_F3, KC_PAUS,                                     _______, _______, _______, _______, _______, _______,
                                                   KC_APP, KC_SPC, KC_TAB, _______, _______, _______
    )
};




/* The default OLED and rotary encoder code can be found at the bottom of qmk_firmware/keyboards/splitkb/kyria/rev1/rev1.c
 * These default settings can be overriden by your own settings in your keymap.c
 * For your convenience, here's a copy of those settings so that you can uncomment them if you wish to apply your own modifications.
 * DO NOT edit the rev1.c file; instead override the weakly defined default functions by your own.
 */

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_180; }

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // Host Keyboard Layer Status
        oled_write_P(PSTR("\nLayer: "), false);
        switch (get_highest_layer(layer_state|default_layer_state)) {
            case _BASE:
                oled_write_P(PSTR("BASE\n"), false);
                break;
            case _NAV:
                oled_write_P(PSTR("NAV\n"), false);
                break;
            case _MEDIA:
                oled_write_P(PSTR("MEDIA\n"), false);
                break;
            case _NUM:
                oled_write_P(PSTR("NUM\n"), false);
                break;
            case _SYM:
                oled_write_P(PSTR("SYM\n"), false);
                break;
            case _FUN:
                oled_write_P(PSTR("FUN\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Write host Keyboard LED Status to OLEDs
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        oled_write_P(PSTR("\n"), false);
        oled_write_P(PSTR("  <3  Agatka  <3\n"), false);
        oled_write_P(PSTR("\n"), false);
        oled_write_P(PSTR("  <3   Emma   <3\n"), false);
    }
    return false;
}
#endif
