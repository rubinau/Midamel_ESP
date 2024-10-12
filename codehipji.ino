#include <SPI.h>
#include <LoRa.h>

// Define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 10

#define SUBMIT_LED 6
#define SUBMIT_BUTTON 18

// TK
#define BUTTON_1 19
#define LED_GRN_1 48
#define LED_YLW_1 47
#define LED_RED_1 20

// SD
#define BUTTON_2 35  // External pull-up required
#define LED_GRN_2 38
#define LED_YLW_2 37
#define LED_RED_2 36

// SMP
#define BUTTON_3 39  // External pull-up required
#define LED_GRN_3 42
#define LED_YLW_3 41
#define LED_RED_3 40

// SMA
#define BUTTON_4 3  // Be cautious, conflicts with UART
#define LED_GRN_4 15
#define LED_YLW_4 16
#define LED_RED_4 17

int counter_1 = 0;
int counter_2 = 0;
int counter_3 = 0;
int counter_4 = 0;


String pesan1 = "";

struct Package {
    int kelasA,kelasB,kelasC,kelasD;
};


String toSTR(Package paket){
    String str;
    str =  String(paket.kelasA) + ";" + String(paket.kelasB) + ";"  + String(paket.kelasC) + ";" + String(paket.kelasD) + ";" ;
    return str;
}

Package paket;
unsigned long submit_timer,send_timer;

bool but1_state, but2_state,but3_state,but4_state, butsub_state;
void updateLEDs(int counter, int ledGreen, int ledYellow, int ledRed) {
  int mod = counter % 4;

  // Turn off all LEDs initially
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);

  // Turn on the appropriate LED based on mod
  if (mod == 1) {
    digitalWrite(ledRed, HIGH);
  } else if (mod == 2) {
    digitalWrite(ledYellow, HIGH);
  } else if (mod == 3) {
    digitalWrite(ledGreen, HIGH);
  }
}

void sendPackets(Package packet){
    updateLEDs(0, LED_GRN_1, LED_YLW_1, LED_RED_1);
    updateLEDs(0, LED_GRN_2, LED_YLW_2, LED_RED_2);
    updateLEDs(0, LED_GRN_3, LED_YLW_3, LED_RED_3);
    updateLEDs(0, LED_GRN_4, LED_YLW_4, LED_RED_4);
    digitalWrite(SUBMIT_LED, HIGH);
    LoRa.beginPacket();
    LoRa.print(toSTR(paket));
    LoRa.endPacket();
    // delay(1500);
    digitalWrite(SUBMIT_LED, LOW);
    // LoRa.beginPacket();
    // LoRa.print("0;0;0;0;");
    // LoRa.endPacket();
    Serial.print("Sent LoRa packet: ");
    Serial.println(toSTR(paket));
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  LoRa.setPins(ss, rst, dio0);
  delay(500);
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF1);
  Serial.println("LoRa Initializing OK!");

  pinMode(SUBMIT_LED, OUTPUT);
  pinMode(SUBMIT_BUTTON, INPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(LED_GRN_1, OUTPUT);
  pinMode(LED_YLW_1, OUTPUT);
  pinMode(LED_RED_1, OUTPUT);

  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(LED_GRN_2, OUTPUT);
  pinMode(LED_YLW_2, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);

  pinMode(BUTTON_3, INPUT_PULLUP); 
  pinMode(LED_GRN_3, OUTPUT);
  pinMode(LED_YLW_3, OUTPUT);
  pinMode(LED_RED_3, OUTPUT);

  pinMode(BUTTON_4, INPUT_PULLUP);
  pinMode(LED_GRN_4, OUTPUT);
  pinMode(LED_YLW_4, OUTPUT);
  pinMode(LED_RED_4, OUTPUT);

  
  }

void loop() {
  if (digitalRead(BUTTON_1) == LOW) {
    counter_1++;
    Serial.print("Button 1 pressed. Counter 1: ");
    Serial.println(counter_1);
    updateLEDs(counter_1, LED_GRN_1, LED_YLW_1, LED_RED_1);
    delay(300);  // Increase debounce delay
  }

  if (digitalRead(BUTTON_2) == LOW) {
    counter_2++;
    Serial.print("Button 2 pressed. Counter 2: ");
    Serial.println(counter_2);
    updateLEDs(counter_2, LED_GRN_2, LED_YLW_2, LED_RED_2);
    delay(300);  // Increase debounce delay
  }

  if (digitalRead(BUTTON_3) == LOW) {
    counter_3++;
    Serial.print("Button 3 pressed. Counter 3: ");
    Serial.println(counter_3);
    updateLEDs(counter_3, LED_GRN_3, LED_YLW_3, LED_RED_3);
    delay(300);  // Increase debounce delay
  }

  if (digitalRead(BUTTON_4) == LOW) {
    counter_4++;
    Serial.print("Button 4 pressed. Counter 4: ");
    Serial.println(counter_4);
    updateLEDs(counter_4, LED_GRN_4, LED_YLW_4, LED_RED_4);
    delay(300);  // Increase debounce delay
  }
    if ((digitalRead(SUBMIT_BUTTON) == LOW)&& (butsub_state == false)) {
        butsub_state = true;
        Serial.println("Sending Packet ....");
        paket.kelasA = counter_1 % 4;
        paket.kelasB = counter_2 % 4;
        paket.kelasC = counter_3 % 4;
        paket.kelasD = counter_4 % 4;
        submit_timer = millis();
        // Reset button counts
        counter_1 = 0;
        counter_2 = 0;
        counter_3 = 0;
        counter_4 = 0;  
    } else if (digitalRead(SUBMIT_BUTTON) == HIGH) {
        butsub_state = false;
        //Serial.println("Submit NOT.");
    }

    if (millis() - submit_timer < 5000){
      if (millis() - submit_timer > 4500){
        paket.kelasA = 0;
        paket.kelasB = 0;
        paket.kelasC = 0;
        paket.kelasD = 0;
        send_timer = millis();
        sendPackets(paket);  
      } else if (millis() - send_timer > 500){
        send_timer = millis();
        sendPackets(paket);      
        }
    } else {
      
      if (millis() - send_timer > 60000){
        paket.kelasA = 0;
        paket.kelasB = 0;
        paket.kelasC = 0;
        paket.kelasD = 0;
        send_timer = millis();
        sendPackets(paket); 
        }
    }

}
