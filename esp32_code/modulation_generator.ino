#include <Arduino.h>
#include <math.h>

// =====================================================
// DAC Output Pin
// =====================================================

#define DAC_PIN 25

// =====================================================
// Mode Selection Pins
// =====================================================

#define MODE_AM_PIN   4
#define MODE_FM_PIN   5
#define MODE_ASK_PIN  18
#define MODE_FSK_PIN  19
#define MODE_PSK_PIN  21

// =====================================================
// Modulation Mode Definitions
// =====================================================

#define MODE_AM   0
#define MODE_FM   1
#define MODE_ASK  2
#define MODE_FSK  3
#define MODE_PSK  4

// =====================================================
// Signal Parameters
// =====================================================

const int fs = 50000;          // 50 kHz Sampling Rate

const int Ncarrier = 128;
const int Nmessage = 512;

const int carrierFreqStep = 3;
const int messageFreqStep = 1;

const int freqDevFM = 5;

// =====================================================
// Lookup Tables
// =====================================================

int8_t carrierTable[Ncarrier];
int16_t messageTable[Nmessage];

// =====================================================
// Runtime Variables
// =====================================================

volatile int ic = 0;
volatile int im = 0;

volatile uint8_t currentMode = MODE_AM;

hw_timer_t *timer = NULL;

// =====================================================
// Build Lookup Tables
// =====================================================

void setupTables()
{
    // Carrier Sine Table
    for (int i = 0; i < Ncarrier; i++)
    {
        carrierTable[i] =
            127 * sin(2 * PI * i / Ncarrier);
    }

    // Message Signal Table
    for (int i = 0; i < Nmessage; i++)
    {
        messageTable[i] =
            32767 * sin(2 * PI * i / Nmessage);
    }
}

// =====================================================
// Read Modulation Mode
// =====================================================

uint8_t readMode()
{
    if (!digitalRead(MODE_AM_PIN))
        return MODE_AM;

    if (!digitalRead(MODE_FM_PIN))
        return MODE_FM;

    if (!digitalRead(MODE_ASK_PIN))
        return MODE_ASK;

    if (!digitalRead(MODE_FSK_PIN))
        return MODE_FSK;

    if (!digitalRead(MODE_PSK_PIN))
        return MODE_PSK;

    return currentMode;
}

// =====================================================
// Timer Interrupt Service Routine
// =====================================================

void IRAM_ATTR onTimer()
{
    uint8_t out = 128;

    int step = carrierFreqStep;

    int16_t analogMsg = messageTable[im];

    // Digital Message derived from sine polarity
    uint8_t digitalMsg = (analogMsg > 0);

    // =================================================
    // AM Modulation
    // =================================================

    if (currentMode == MODE_AM)
    {
        int16_t y =
            (analogMsg >> 8) * carrierTable[ic];

        y >>= 7;

        out = constrain(y + 128, 0, 255);
    }

    // =================================================
    // FM Modulation
    // =================================================

    else if (currentMode == MODE_FM)
    {
        step =
            carrierFreqStep +
            (analogMsg * freqDevFM) / 32768;

        ic += step;

        if (ic >= Ncarrier)
            ic -= Ncarrier;

        out = carrierTable[ic] + 128;
    }

    // =================================================
    // ASK Modulation
    // =================================================

    else if (currentMode == MODE_ASK)
    {
        out =
            digitalMsg ?
            carrierTable[ic] + 128 :
            0;
    }

    // =================================================
    // FSK Modulation
    // =================================================

    else if (currentMode == MODE_FSK)
    {
        step = digitalMsg ? 6 : 2;

        ic += step;

        if (ic >= Ncarrier)
            ic -= Ncarrier;

        out = carrierTable[ic] + 128;
    }

    // =================================================
    // PSK Modulation
    // =================================================

    else if (currentMode == MODE_PSK)
    {
        int idx =
            digitalMsg ?
            (ic + Ncarrier / 2) % Ncarrier :
            ic;

        out = carrierTable[idx] + 128;
    }

    // =================================================
    // DAC Output
    // =================================================

    dacWrite(DAC_PIN, out);

    // =================================================
    // Phase Updates
    // =================================================

    ic += carrierFreqStep;

    if (ic >= Ncarrier)
        ic -= Ncarrier;

    im += messageFreqStep;

    if (im >= Nmessage)
        im = 0;
}

// =====================================================
// Setup
// =====================================================

void setup()
{
    pinMode(MODE_AM_PIN, INPUT_PULLUP);
    pinMode(MODE_FM_PIN, INPUT_PULLUP);
    pinMode(MODE_ASK_PIN, INPUT_PULLUP);
    pinMode(MODE_FSK_PIN, INPUT_PULLUP);
    pinMode(MODE_PSK_PIN, INPUT_PULLUP);

    setupTables();

    timer = timerBegin(fs);

    timerAttachInterrupt(timer, &onTimer);

    timerAlarm(timer, 1, true, 0);
}

// =====================================================
// Main Loop
// =====================================================

void loop()
{
    currentMode = readMode();
}
