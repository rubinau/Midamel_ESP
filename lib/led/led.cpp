#include <Arduino.h>
#include <led.h>

// Constructor to initialize the RGB pins and active high/low setting
LedRGB::LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isActiveHigh) 
    : redPin(rPin), greenPin(gPin), bluePin(bPin), activeHigh(isActiveHigh) {
        redPin = rPin;
        bluePin = bPin;
        greenPin = gPin;
        activeHigh = isActiveHigh;
}

// Private method to write to a pin based on activeHigh setting

void LedRGB::setupLed(){
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    // Initialize all pins to LOW (off)
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
}

void LedRGB::writePin(uint8_t pin, bool state) {
    digitalWrite(pin, activeHigh ? state : !state);
}

// Function to enable the red component
void LedRGB::enableRed(bool state) {
    writePin(redPin, state);
}

// Function to enable the green component
void LedRGB::enableGreen(bool state) {
    writePin(greenPin, state);
}

// Function to enable the blue component
void LedRGB::enableBlue(bool state) {
    writePin(bluePin, state);
}

// Function to disable all components
void LedRGB::disableAll() {
    writePin(redPin, LOW);
    writePin(greenPin, LOW);
    writePin(bluePin, LOW);
}