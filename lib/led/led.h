#pragma once
#include <Arduino.h>

class LedRGB {
private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;
    bool activeHigh;
    int kelas;

    void writePin(uint8_t pin, bool state);

public:
    // Constructor to initialize the RGB pins and active high/low setting
    LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isActiveHigh, int kelaz);

    void setupLed();

    // Function to enable the red component
    void enableRed(bool state);

    // Function to enable the green component
    void enableGreen(bool state);

    // Function to enable the blue component
    void enableBlue(bool state);

    // Function to disable all components
    void disableAll();

    void updateState(int count);

    void updateFromString(int kelas_tk, int kelas_sd, int kelas_smp, int kelas_sma);
};

typedef struct
{
    int pin;
    int times;
    int delay_time;
    bool active_high;
}led_param;

void callBlinkLed(int led, int times, int delay_time, bool activeHigh);
