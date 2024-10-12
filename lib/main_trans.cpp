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

LedRGB led_01(LED_01_RED,LED_01_GRN,LED_01_YLW,true,0);
LedRGB led_02(LED_02_RED,LED_02_GRN,LED_02_YLW,true,0);
LedRGB led_03(LED_03_RED,LED_03_GRN,LED_03_YLW,true,0);
LedRGB led_04(LED_04_RED,LED_04_GRN,LED_04_YLW,true,0);

LoRaClass thisLoRa;

unsigned long last_time, last_submit;
void setup(){
    // LORA
    SPIClass customSPI(1);
    customSPI.begin(LoRa_SCK, LoRa_MISO, LoRa_MOSI, LoRa_NSS);
    // Set custom SPI and pins for LoRa
    thisLoRa.setSPI(customSPI);
    thisLoRa.setPins(LoRa_NSS, LoRa_RST, LoRa_DI00); 
    Serial.begin(115200);
    Serial.println("LoRa Sender");
     
    while (!thisLoRa.begin(433E6)) {
      Serial.println(".");
      delay(500);
    }
    thisLoRa.setSyncWord(0xF1);
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
    last_submit = 0;
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
        Serial.println("Button Submit pressed");
        thisLoRa.beginPacket();
        Serial.println("Button Submit pressedeed");
        paket.kelasA = but1_count % 4;
        paket.kelasB = but2_count % 4;
        paket.kelasC = but3_count % 4;
        paket.kelasD = but4_count % 4;
        String packet;
        packet = toSTR(paket);
        Serial.println(packet);
        thisLoRa.print(packet);
        thisLoRa.endPacket();
        last_submit = millis();
        but1_count = 0;
        but2_count = 0;
        but3_count = 0;
        but4_count = 0;
        callBlinkLed(Submit_LED,10,500,true);
        //digitalWrite(Submit_LED, 1);
    } else if (digitalRead(Submit_Button) == HIGH) {
        butsub_on = false;
        butsub_state = false;
        //digitalWrite(Submit_LED, 0);
        //Serial.println("Button 4 not");
    }

    if (millis() - last_time > 10000){
        led_01.disableAll();
        led_02.disableAll();
        led_03.disableAll();
        led_04.disableAll();
        // led_01.updateState(paket.kelasA);
        // led_02.updateState(paket.kelasB);
        // led_03.updateState(paket.kelasC);
        // led_04.updateState(paket.kelasD);
    } else {
        led_01.updateStatePassive(but1_count);
        led_02.updateStatePassive(but2_count);
        led_03.updateStatePassive(but3_count);
        led_04.updateStatePassive(but4_count);
    }

    if (last_submit - millis() < 3000) {
        //thisLoRa.beginPacket();
        // thisLoRa.print(packet);
        //thisLoRa.endPacket();
        delay(500);
    } else {
        paket.kelasA = but1_count % 4;
        paket.kelasB = but2_count % 4;
        paket.kelasC = but3_count % 4;
        paket.kelasD = but4_count % 4;
        // packet = toSTR(paket);
    }
    //digitalWrite(Submit_LED, 1);
    delay(100);
}