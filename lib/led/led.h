#pragma once
#include <Arduino.h>

class LedRGB {
private:
    int redPin;
    int greenPin;
    int bluePin;
    bool activeHigh;
    int kelas;

    int kelasA;
    int kelasB;
    int kelasC;
    int kelasD;

    void writePin(int pin, bool state);

public:
    // Constructor to initialize the RGB pins and active high/low setting
    LedRGB(int rPin, int gPin, int bPin, bool isActiveHigh, int kelaz);

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
    int pin1;
    int pin2;
    int times;
    int delay_time;
    bool active_high;
}led_param;

void callBlinkLed(int led, int times, int delay_time, bool activeHigh);
void callBlinkLed(int led1, int led2, int times, int delay_time, bool activeHigh);
