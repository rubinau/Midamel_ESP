#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <led.h> 
#include <pin_unit.h>

// Create an instance of the LedRGB class
LedRGB led1(SSR_1_CH1, SSR_1_CH2, SSR_1_CH3, false);
LedRGB led2(SSR_2_CH1, SSR_2_CH2, SSR_2_CH3, false);



void setup() {
    // Initialize Serial
    Serial.begin(115200);
    Serial.println("LoRa Receiver");
    uint64_t chipid = ESP.getEfuseMac();
    Serial.printf("CHIP ID = %04X", (uint16_t)(chipid >> 32));
    Serial.printf("%08X\n", (uint32_t)chipid);
    
    // Initialize LoRa
    LoRa.setPins(LoRa_NSS, LoRa_RST, LoRa_DI00);
    if (!LoRa.begin(433E6)) {
        Serial.println("Failed to initialize LoRa.");
        while (1);
    }
    LoRa.setSyncWord(0xF1);
    Serial.println("LoRa Initializing Successful!");

    led1.setupLed();
    led1.disableAll();
    led2.setupLed();
    led2.disableAll();
}


void loop() {
    // Check if a packet is available
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // Read the packet
        String packet = "";
        while (LoRa.available()) {
            packet += (char)LoRa.read();
        }
        
        // Print received packet for debugging
        Serial.print("Received packet: ");
        Serial.println(packet);

        // Parse the packet
        // Expected format: "1;0;0;0;" or similar
        int separator1 = packet.indexOf(';');
        int separator2 = packet.indexOf(';', separator1 + 1);
        int separator3 = packet.indexOf(';', separator2 + 1);

        if (separator1 != -1 && separator2 != -1 && separator3 != -1) {
            String redStr = packet.substring(0, separator1);
            String greenStr = packet.substring(separator1 + 1, separator2);
            String blueStr = packet.substring(separator2 + 1, separator3);
        
            int redValue = redStr.toInt();
            int greenValue = greenStr.toInt();
            int blueValue = blueStr.toInt();

            // Set the LED color
            // Map the received values to either HIGH or LOW as needed
            led1.enableRed(redValue == 1);
            led1.enableGreen(greenValue == 1);
            led1.enableBlue(blueValue == 1);

            led2.enableRed(redValue == 1);
            led2.enableGreen(greenValue == 1);
            led2.enableBlue(blueValue == 1);
        }
    }

    delay(100); // Small delay to avoid excessive LoRa checking
}
