#pragma once

#define ENCODER_MAP_LAYERS        1
#define ENCODER_ENABLE_INTERRUPTS 0
#define ENCODER_MAP_KEY_DELAY     5

#define USB_POLLING_INTERVAL_MS   1

#define OLED_DRIVER_ENABLE
#define OLED_DISPLAY_128X64

// Use software I2C on PD0 (SCL) / PD1 (SDA):
#define I2C_DRIVER            I2C_SOFTWARE
#define I2C_SCL_PIN           PD0
#define I2C_SDA_PIN           PD1

#define RAW_ENABLE
#define RAW_EPSIZE 64