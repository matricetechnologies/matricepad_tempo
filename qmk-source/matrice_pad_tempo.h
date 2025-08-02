#pragma once
#include "quantum.h"

#define LAYOUT_2x2_knob( \
    k00,k01,     \
    k02,k03,     \
    kbtn                 \
) {                      \
    { k00, k01, }, \
    { k02, k03, }, \
    { kbtn, KC_NO } \
}
