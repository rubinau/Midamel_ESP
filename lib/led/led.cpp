#include <Arduino.h>
#include <led.h>

class LedRGB {
private:
    uint8_t redPin;
    uint8_t greenPin;
    uint8_t bluePin;
    bool activeHigh;

    void writePin(uint8_t pin, bool state) {
        digitalWrite(pin, activeHigh ? state : !state);
    }

public:
    // Constructor to initialize the RGB pins and active high/low setting
    LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isActiveHigh) 
        : redPin(rPin), greenPin(gPin), bluePin(bPin), activeHigh(isActiveHigh) {
        pinMode(redPin, OUTPUT);
        pinMode(greenPin, OUTPUT);
        pinMode(bluePin, OUTPUT);

        // Initialize all pins to LOW (off)
        writePin(redPin, LOW);
        writePin(greenPin, LOW);
        writePin(bluePin, LOW);
    }

    // Function to enable the red component
    void enableRed(bool state) {
        writePin(redPin, state);
    }

    // Function to enable the green component
    void enableGreen(bool state) {
        writePin(greenPin, state);
    }

    // Function to enable the blue component
    void enableBlue(bool state) {
        writePin(bluePin, state);
    }

    // Function to disable all components
    void disableAll() {
        writePin(redPin, LOW);
        writePin(greenPin, LOW);
        writePin(bluePin, LOW);
    }
};

// Example usage
// LedRGB led(9, 10, 11, true); // Pins 9, 10, 11 for RGB, active high
// led.enableRed(true); // Turn on red component
// led.enableGreen(true); // Turn on green component
// led.enableBlue(true); // Turn on blue component
// led.disableAll(); // Turn off all components


