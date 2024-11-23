#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Include the LCD I2C library

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

unsigned long lastButtonPressTime = 0;  // Timer to track inactivity
unsigned long submit_timer = 0, send_timer = 0;

bool changesDetected = false;  // Track if any changes occurred
bool submitState = false;      // Track if submit button is pressed
bool butsub_state = false;

// LoRa package structure
struct Package {
  int kelasA, kelasB, kelasC, kelasD;
};

Package paket;

// LCD initialization with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

void updateLCD(const char* line1, const char* line2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

String getState(int counter) {
  int mod = counter % 4;
  if (mod == 0) return "Tidak diset";
  if (mod == 1) return "Merah";
  if (mod == 2) return "Kuning";
  return "Hijau";
}

void displayClassStates() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TK: " + getState(counter_1));
  lcd.setCursor(0, 1);
  lcd.print("SD: " + getState(counter_2));
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SMP: " + getState(counter_3));
  lcd.setCursor(0, 1);
  lcd.print("SMA: " + getState(counter_4));
  delay(2000);
}

String toSTR(Package paket) {
  return String(paket.kelasA) + ";" + String(paket.kelasB) + ";" +
         String(paket.kelasC) + ";" + String(paket.kelasD) + ";";
}

void sendPackets(Package packet) {
  updateLEDs(0, LED_GRN_1, LED_YLW_1, LED_RED_1);
  updateLEDs(0, LED_GRN_2, LED_YLW_2, LED_RED_2);
  updateLEDs(0, LED_GRN_3, LED_YLW_3, LED_RED_3);
  updateLEDs(0, LED_GRN_4, LED_YLW_4, LED_RED_4);
  digitalWrite(SUBMIT_LED, HIGH);
  
  LoRa.beginPacket();
  LoRa.print(toSTR(packet));
  LoRa.endPacket();
  
  digitalWrite(SUBMIT_LED, LOW);
  
  Serial.print("Sent LoRa packet: ");
  Serial.println(toSTR(packet));
}

void updateLEDs(int counter, int ledGreen, int ledYellow, int ledRed) {
  int mod = counter % 4;
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed, LOW);

  if (mod == 1) digitalWrite(ledRed, HIGH);
  else if (mod == 2) digitalWrite(ledYellow, HIGH);
  else if (mod == 3) digitalWrite(ledGreen, HIGH);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Retrying...");
    delay(500);
  }
  LoRa.setSyncWord(0xF1);
  Serial.println("LoRa Initialized!");

  pinMode(SUBMIT_LED, OUTPUT);
  pinMode(SUBMIT_BUTTON, INPUT_PULLUP);

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

  lcd.begin(16,2);
  lcd.backlight();
  updateLCD("Tekan tombol", "set mode");
}

void loop() {
  bool buttonPressed = false;

  if (digitalRead(BUTTON_1) == LOW) {
    counter_1++;
    updateLEDs(counter_1, LED_GRN_1, LED_YLW_1, LED_RED_1);
    buttonPressed = true;
    delay(300);
  }

  if (digitalRead(BUTTON_2) == LOW) {
    counter_2++;
    updateLEDs(counter_2, LED_GRN_2, LED_YLW_2, LED_RED_2);
    buttonPressed = true;
    delay(300);
  }

  if (digitalRead(BUTTON_3) == LOW) {
    counter_3++;
    updateLEDs(counter_3, LED_GRN_3, LED_YLW_3, LED_RED_3);
    buttonPressed = true;
    delay(300);
  }

  if (digitalRead(BUTTON_4) == LOW) {
    counter_4++;
    updateLEDs(counter_4, LED_GRN_4, LED_YLW_4, LED_RED_4);
    buttonPressed = true;
    delay(300);
  }

  if (buttonPressed) {
    lastButtonPressTime = millis();
    changesDetected = true;
    displayClassStates();
  }

  if (digitalRead(SUBMIT_BUTTON) == LOW && !butsub_state) {
    butsub_state = true;
    paket.kelasA = counter_1 % 4;
    paket.kelasB = counter_2 % 4;
    paket.kelasC = counter_3 % 4;
    paket.kelasD = counter_4 % 4;

    sendPackets(paket);

    updateLCD("Data Sent", "Sending...");
    delay(1000);
    
    counter_1 = counter_2 = counter_3 = counter_4 = 0;
    changesDetected = false;
  } else if (digitalRead(SUBMIT_BUTTON) == HIGH) {
    butsub_state = false;
  }

  if (!changesDetected && millis() - lastButtonPressTime > 10000) {
    updateLCD("Tekan tombol", "set mode");
  }

  if (changesDetected && millis() - lastButtonPressTime > 10000 && !submitState) {
    updateLCD("Tekan tombol", "submit (biru)");
  }
}
