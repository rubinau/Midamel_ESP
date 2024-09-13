#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <led.h> 
#include <pin_unit.h>

// LedRGB class
LedRGB led1(SSR_1_CH1, SSR_1_CH2, SSR_1_CH3, false,0);
LedRGB led2(SSR_2_CH1, SSR_2_CH2, SSR_2_CH3, false,0);

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
    led1.setupLed();
    led2.setupLed();
    led1.disableAll();
    led2.disableAll();
}

void loop() {
    static unsigned long lastToggleTime = 0;
    static int currentColor = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastToggleTime >= 1000) {
        lastToggleTime = currentTime;
        
        char buff[50];
        sprintf(buff, "TICK %d", currentColor);
        Serial.println(buff);
        led1.disableAll();
        led2.disableAll();
        // Toggle RGB
        if (currentColor == 0) {
            led1.enableRed(true);
            led2.enableRed(true);
        } else if (currentColor == 1) {
            led1.enableGreen(true);
            led2.enableGreen(true);
        } else if (currentColor == 2) {
            led1.enableBlue(true);
            led2.enableBlue(true);
        }

        currentColor = (currentColor + 1) % 3;
    }
}
