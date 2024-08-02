#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <led.h> 
#include <pin_unit.h>

// Create an instance of the LedRGB class
LedRGB led(SSR_1_CH1, SSR_1_CH2, SSR_1_CH3, false); // Adjust the pins according to your setup

void setup() {
    // Initialize Serial
    Serial.begin(115200);
    Serial.println("LoRa Receiver");

    // Initialize LoRa
    LoRa.setPins(LoRa_NSS, LoRa_RST, LoRa_DI00);
    if (!LoRa.begin(433E6)) {
        Serial.println("Failed to initialize LoRa.");
        while (1);
    }
    LoRa.setSyncWord(0xF1);
    Serial.println("LoRa Initializing Successful!");

    // Initialize RGB LED
    led.disableAll(); // Ensure the LED is off initially
}

void loop() {
    static unsigned long lastToggleTime = 0;
    static int currentColor = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastToggleTime >= 1000) {
        lastToggleTime = currentTime;
        led.disableAll();

        // Toggle RGB
        if (currentColor == 0) {
            led.enableRed(true);
        } else if (currentColor == 1) {
            led.enableGreen(true);
        } else if (currentColor == 2) {
            led.enableBlue(true);
        }

        currentColor = (currentColor + 1) % 3;
    }

    delay(100);
}
