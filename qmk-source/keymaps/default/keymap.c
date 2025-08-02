#include QMK_KEYBOARD_H
#include "print.h"

/* ── existing F6 pull-up & matrix code here ── */
void keyboard_pre_init_user(void) { setPinInputHigh(F6); }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_2x2_knob(
        KC_MPRV, KC_MPLY,
        KC_MSTP, KC_MNXT,
        KC_MUTE
    )
};
const uint16_t PROGMEM encoder_map[1][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { { KC_VOLD, KC_VOLU } }
};

void matrix_scan_user(void) {
    // ── Your existing F6⇒mute code ──
    static bool last = true;
    bool now = readPin(F6);
    if (now != last) {
        last = now;
        if (!now) tap_code16(KC_AUDIO_MUTE);
    }

    // ── New: Raw HID receive & parse ──
    static uint8_t hid_buf[64];
    if (raw_hid_receive(hid_buf, sizeof(hid_buf))) {
        // Copy payload into a C-string
        char buf[64];
        memcpy(buf, hid_buf, 63);
        buf[63] = '\0';

        // Look for "||" separators
        char *first = strstr(buf, "||");
        char *second = first ? strstr(first + 2, "||") : NULL;
        if (first && second) {
            *first = *second = '\0';
            // Title = buf
            // Artist = first+2
            // VolumeStr = second+2
            snprintf(songTitle, sizeof(songTitle), "%s", buf);
            snprintf(artist,   sizeof(artist),   "%s", first + 2);
            volume = atoi(second + 2);
            volumeInitialized = true;
            lastUpdate = timer_read();
        }
    }
}

// ── Global state for display ──
static char songTitle[32];
static char line1[29], line2[29];
static char artist[29];
static bool volumeInitialized = false;
static uint8_t volume = 0;
static uint16_t lastUpdate = 0;

/* ── OLED drawing ── */
bool oled_task_user(void) {
    // Clear if no data yet
    if (!volumeInitialized) return false;

    oled_clear();
    // splitTitleIntoLines is your helper from Arduino — you can port it, or just:
    // e.g., strncpy(line1, songTitle, 28); line1[28]=0; // simple wrap

    oled_set_cursor(0, 0);
    oled_write_ln(line1, false);
    oled_write_ln(line2, false);

    oled_set_cursor(0, 3);
    oled_write_ln(artist, false);

    // Optionally show a volume bar:
    oled_set_cursor(0, 7);
    for (uint8_t i = 0; i < (volume * 10 / 100); i++) {
        oled_write_char('|', false);
    }

    // Auto-clear after 5s of no updates:
    if (timer_elapsed(lastUpdate) > 5000) {
        volumeInitialized = false;
    }
    return false;
}
