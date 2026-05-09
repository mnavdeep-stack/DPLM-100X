#include <Arduino.h>
#include "driver/gpio.h"

// =====================================================
// Pin Definitions
// =====================================================

#define SINE_GEN_PIN      26
#define ANALOG_IN_PIN     34
#define DIGITAL_MSG_PIN   27
#define DRDY_PIN          32

// =====================================================
// Parallel Bus GPIO Mask
// =====================================================

const uint64_t BUS_MASK =
    (0xFF << 12) |
    (1ULL << 21) |
    (1ULL << 22) |
    (1ULL << 23) |
    (1ULL << 25);

// =====================================================
// Timer Variables
// =====================================================

hw_timer_t *timer = NULL;

volatile int phase = 0;
volatile int dig_count = 0;

// =====================================================
// Timer Interrupt Routine
// =====================================================

void IRAM_ATTR onTimer()
{
    // ---------------------------------------------
    // Internal Digital Message Generation
    // ---------------------------------------------

    bool internal_dig = (dig_count < 500);

    digitalWrite(DIGITAL_MSG_PIN, internal_dig);

    if (++dig_count >= 1000)
        dig_count = 0;

    // ---------------------------------------------
    // Analog Sine Wave Generation
    // ---------------------------------------------

    uint8_t s_val =
        127 + 127 * sin(phase * 0.0628);

    dacWrite(SINE_GEN_PIN, s_val);

    phase = (phase + 1) % 100;

    // ---------------------------------------------
    // ADC Sampling
    // ---------------------------------------------

    uint16_t adc_val =
        analogRead(ANALOG_IN_PIN);

    // ---------------------------------------------
    // Parallel Bus Output
    // ---------------------------------------------

    uint32_t port_bits =
        ((adc_val & 0xFF) << 12);

    if (adc_val & (1 << 8))
        port_bits |= (1 << 21);

    if (adc_val & (1 << 9))
        port_bits |= (1 << 22);

    if (adc_val & (1 << 10))
        port_bits |= (1 << 23);

    if (adc_val & (1 << 11))
        port_bits |= (1 << 25);

    REG_WRITE(GPIO_OUT_W1TS_REG, port_bits);
    REG_WRITE(GPIO_OUT_W1TC_REG,
             (~port_bits) & BUS_MASK);

    // ---------------------------------------------
    // Data Ready Strobe
    // ---------------------------------------------

    digitalWrite(DRDY_PIN, HIGH);
    digitalWrite(DRDY_PIN, LOW);
}

// =====================================================
// Setup
// =====================================================

void setup()
{
    pinMode(DIGITAL_MSG_PIN, OUTPUT);
    pinMode(DRDY_PIN, OUTPUT);

    analogReadResolution(12);

    gpio_config_t io_conf = {
        .pin_bit_mask = BUS_MASK,
        .mode = GPIO_MODE_OUTPUT
    };

    gpio_config(&io_conf);

    timer = timerBegin(10000);

    timerAttachInterrupt(timer, &onTimer);

    timerAlarm(timer, 1, true, 0);
}

// =====================================================
// Main Loop
// =====================================================

void loop()
{
    // Real-time processing handled in ISR
}
