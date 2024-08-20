#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <led.h>
#include <pin_controller.h>

String pesan1 = "";

struct Package {
    int kelasA,kelasB,kelasC,kelasD;
};


String toSTR(Package paket){
    String str;
    str =  String(paket.kelasA) + ";" + String(paket.kelasB) + ";"  + String(paket.kelasC) + ";" + String(paket.kelasD) + ";" ;
    return str;
}

LedRGB led_01(LED_01_RED,LED_01_GRN,LED_01_YLW,true);
LedRGB led_02(LED_02_RED,LED_02_GRN,LED_02_YLW,true);
LedRGB led_03(LED_03_RED,LED_03_GRN,LED_03_YLW,true);
LedRGB led_04(LED_04_RED,LED_04_GRN,LED_04_YLW,true);

void setup(){
    // LORA
    SPIClass customSPI(0);
    customSPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_NSS);
    // Set custom SPI and pins for LoRa
    LoRa.setSPI(customSPI);
    LoRa.setPins(LoRa_NSS, LoRa_RST, LoRa_DI00); 
    Serial.begin(115200);
    Serial.println("LoRa Sender");
     
    // while (!LoRa.begin(433E6)) {
    //   Serial.println(".");
    //   delay(500);
    // }
    // LoRa.setSyncWord(0xF1);
    // Serial.println("LoRa Initializing Successful!");

    // CONTROL BUTTON
    led_01.setupLed();
    led_02.setupLed();
    led_03.setupLed();
    led_04.setupLed();
    pinMode(Button_01, INPUT_PULLUP);
    pinMode(Button_02, INPUT_PULLUP);
    pinMode(Button_03, INPUT_PULLUP);
    pinMode(Button_04, INPUT_PULLUP);
    pinMode(Submit_Button, INPUT_PULLUP);
    pinMode(Submit_LED, OUTPUT);
}

Package paket;
String state_l1, state_l2, state_l3, state_l4;
bool but1_state, but2_state,but3_state,but4_state, butsub_state;
int but1_count, but2_count, but3_count, but4_count, butsub_on;
void loop(){
    // Handle Button 1
    if (digitalRead(Button_01) == LOW && !but1_state) {
        but1_state = true;
        but1_count++;
        Serial.println("Button 1 pressed");
    } else if (digitalRead(Button_01) == HIGH) {
        but1_state = false;
    }
    
    // Update LED 1 based on button 1 state
    switch (but1_count % 4) {
        case 0: led_01.disableAll(); state_l1 = "0"; break;
        case 1: led_01.enableRed(true); state_l1 = "1"; break;
        case 2: led_01.enableGreen(true); state_l1 = "2"; break;
        case 3: led_01.enableBlue(true); state_l1 = "3"; break;
    }

    // Handle Button 2
    if (digitalRead(Button_02) == LOW && !but2_state) {
        but2_state = true;
        but2_count++;
        Serial.println("Button 2 pressed");
    } else if (digitalRead(Button_02) == HIGH) {
        but2_state = false;
    }

    // Update LED 2 based on button 2 state
    switch (but2_count % 4) {
        case 0: led_02.disableAll(); state_l2 = "0"; break;
        case 1: led_02.enableRed(true); state_l2 = "1"; break;
        case 2: led_02.enableGreen(true); state_l2 = "2"; break;
        case 3: led_02.enableBlue(true); state_l2 = "3"; break;
    }

    // Handle Button 3
    if (digitalRead(Button_03) == LOW && !but3_state) {
        but3_state = true;
        but3_count++;
        Serial.println("Button 3 pressed");
    } else if (digitalRead(Button_03) == HIGH) {
        but3_state = false;
    }

    // Update LED 3 based on button 3 state
    switch (but3_count % 4) {
        case 0: led_03.disableAll(); state_l3 = "0"; break;
        case 1: led_03.enableRed(true); state_l3 = "1"; break;
        case 2: led_03.enableGreen(true); state_l3 = "2"; break;
        case 3: led_03.enableBlue(true); state_l3 = "3"; break;
    }

    // Handle Button 4
    if (digitalRead(Button_04) == LOW && !but4_state) {
        but4_state = true;
        but4_count++;
        Serial.println("Button 4 pressed");
    } else if (digitalRead(Button_04) == HIGH) {
        but4_state = false;
    }

    // Update LED 4 based on button 4 state
    switch (but4_count % 4) {
        case 0: led_04.disableAll(); state_l4 = "0"; break;
        case 1: led_04.enableRed(true); state_l4 = "1"; break;
        case 2: led_04.enableGreen(true); state_l4 = "2"; break;
        case 3: led_04.enableBlue(true); state_l4 = "3"; break;
    }
    but1_count++;
    but2_count++;
    but3_count++;
    but4_count++;
    Serial.println("PING");
    paket.kelasA = but1_count % 4;
    paket.kelasB = but2_count % 4;
    paket.kelasC = but3_count % 4;
    paket.kelasD = but4_count % 4;

    delay(100);
    // Handle Submit Button
    // if (digitalRead(Submit_Button) == LOW && !butsub_state) {
        // butsub_state = true;
        // butsub_on = 5000;

        // LoRa.beginPacket();
        // LoRa.print(toSTR(paket));
        // LoRa.endPacket();
        Serial.print("Sent LoRa packet: ");
        Serial.println(toSTR(paket));

    //     // Reset button counts
    //     but1_count = 0;
    //     but2_count = 0;
    //     but3_count = 0;
    //     but4_count = 0;
        
    // } else if (digitalRead(Submit_Button) == HIGH) {
    //     butsub_state = false;
    // }

    // Update Submit LED
    if (butsub_on != 0) {
        digitalWrite(Submit_LED, HIGH);
        butsub_on--;
    } else {
        digitalWrite(Submit_LED, LOW);
    }

    delay(1);  
}