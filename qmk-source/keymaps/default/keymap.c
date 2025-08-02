#include QMK_KEYBOARD_H
#include "print.h"

void keyboard_pre_init_user(void) { setPinInputHigh(F6); }   // pull-up

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_2x2_knob(
        KC_F13, KC_F14,
        KC_F15, KC_F16
        KC_MUTE                    /* kbtn → encoder press */
    )
};
/* rotary turns (direction already correct) */
const uint16_t PROGMEM encoder_map[1][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { { KC_VOLD, KC_VOLU } }
};

/* ── make F6 act as a key ─────────────────────────────────────────── */
void matrix_scan_user(void) {
    static bool last = true;            // start HIGH (pull-up)
    bool now = readPin(F6);             // LOW = pressed
    if (now != last) {
        last = now;
        if (!now) {                     // went LOW → key down
            tap_code16(KC_AUDIO_MUTE);  // send mute
        }
    }
}
