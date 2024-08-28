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

unsigned long last_time;
void setup(){
    // LORA
    SPIClass customSPI(1);
    customSPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_NSS);
    // Set custom SPI and pins for LoRa
    LoRa.setSPI(customSPI);
    LoRa.setPins(LoRa_NSS, LoRa_RST, LoRa_DI00); 
    Serial.begin(115200);
    Serial.println("LoRa Sender");
     
    while (!LoRa.begin(433E6)) {
      Serial.println(".");
      delay(500);
    }
    LoRa.setSyncWord(0xF1);
    Serial.println("LoRa Initializing Successful!");

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
    last_time = 0;
}

Package paket;
String state_l1, state_l2, state_l3, state_l4;
bool but1_state, but2_state,but3_state,but4_state, butsub_state;
int but1_count = 0, but2_count = 0, but3_count = 0, but4_count = 0, butsub_on = 0;
void loop(){
    // Handle Button 1
    // Serial.println("..");
    if ((digitalRead(Button_01) == LOW) && (but1_state == false)) {
        but1_state = true;
        but1_count++;
        Serial.println("Button 1 pressed");
        last_time = millis();
    } else if (digitalRead(Button_01) == HIGH) {
        but1_state = false;
        //  Serial.println("Button 1 not");
    }
    
    // Handle Button 2
    if ((digitalRead(Button_02) == LOW) && (but2_state == false)) {
        but2_state = true;
        but2_count++;
        Serial.println("Button 2 pressed");
        last_time = millis();
    } else if (digitalRead(Button_02) == HIGH) {
        but2_state = false;
        // Serial.println("Button 2 not");
    }
    // Handle Button 3
    if ((digitalRead(Button_03) == LOW) && (but3_state == false)) {
        but3_state = true;
        but3_count++;
        Serial.println("Button 3 pressed");
        last_time = millis();
    } else if (digitalRead(Button_03) == HIGH) {
        but3_state = false;
        //  Serial.println("Button 3 not");
    }
    // Handle Button 4
    if ((digitalRead(Button_04) == LOW ) && (but4_state == false)) {
        but4_state = true;
        but4_count++;
        Serial.println("Button 4 pressed");
        last_time = millis();
    } else if (digitalRead(Button_04) == HIGH) {
        but4_state = false;
        //  Serial.println("Button 4 not");
    }
    // Handle Button Submit
    if ((digitalRead(Submit_Button) == LOW ) && (butsub_state == false)) {
        butsub_state = true;
        butsub_on = 1;
        // Send the packet
        LoRa.beginPacket();
        Serial.println("Button Submit pressed");
        paket.kelasA = but1_count % 4;
        paket.kelasB = but2_count % 4;
        paket.kelasC = but3_count % 4;
        paket.kelasD = but4_count % 4;
        String packet = toSTR(paket);
        Serial.println(packet);
        LoRa.print(packet);
        LoRa.endPacket();
    } else if (digitalRead(Submit_Button) == HIGH) {
        butsub_on = false;
        //  Serial.println("Button 4 not");
    }

    if (millis() - last_time > 5000){
        led_01.disableAll();
        led_02.disableAll();
        led_03.disableAll();
        led_04.disableAll();
        // led_01.updateState(paket.kelasA);
        // led_02.updateState(paket.kelasB);
        // led_03.updateState(paket.kelasC);
        // led_04.updateState(paket.kelasD);
    } else {
        led_01.updateState(but1_count);
        led_02.updateState(but2_count);
        led_03.updateState(but3_count);
        led_04.updateState(but4_count);
    }
    digitalWrite(Submit_LED, butsub_on);
    delay(100);
}