#include <Arduino.h>
#include <led.h>

// Constructor to initialize the RGB pins and active high/low setting
LedRGB::LedRGB(uint8_t rPin, uint8_t gPin, uint8_t bPin, bool isActiveHigh,int kelaz) 
    : redPin(rPin), greenPin(gPin), bluePin(bPin), activeHigh(isActiveHigh) {
        redPin = rPin;
        bluePin = bPin;
        greenPin = gPin;
        activeHigh = isActiveHigh;
        kelas = kelaz;
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

void LedRGB::updateState(int count) {
    switch (count % 4) {
        case 0:
            disableAll();  // Null state, all LEDs off
            break;
        
        case 1:
            enableRed(false);
            //enableGreen(true); // Green state
            callBlinkLed(greenPin,5,200,activeHigh);
            enableBlue(false);
            break;
        case 2:
            enableRed(false);
            enableGreen(false);
            //enableBlue(true); // Blue state
            callBlinkLed(bluePin,5,200,activeHigh);
            break;
        case 3:
            //enableRed(true); // Red state
            callBlinkLed(redPin,5,200,activeHigh);
            enableGreen(false);
            enableBlue(false);
            break;
        default:
            break;
    }
}

void LedRGB::updateFromString(int count_tk, int count_sd, int count_smp, int count_sma){
    switch (kelas)
    {
    case 0:
        updateState(count_tk);
        break;
    case 1:
        updateState(count_sd);
        break;
    case 2:
        updateState(count_smp);
        break;
    case 3:
        updateState(count_sma);
        break;
    default:
        break;
    }
}

void blinkLED(int led, int times, int delay_time, bool active_high)
{
    for (int i = 0; i < times; i++)
    {
        digitalWrite(led, active_high);
        vTaskDelay(delay_time);
        digitalWrite(led, !active_high);
        vTaskDelay(delay_time);
    }
    digitalWrite(led,active_high);
    vTaskDelay(3000);
    digitalWrite(led,!active_high);
}

void vTaskLED(void *parameter)
{
    led_param params = *((led_param *)parameter);
    blinkLED(params.pin,params.times,params.delay_time,params.active_high);
    vTaskDelete(NULL);
}

void callBlinkLed(int led, int times, int delay_time, bool active_high)
{
    led_param submit;
    submit.pin = led;
    submit.times = times;
    submit.delay_time = delay_time;
    submit.active_high = active_high;
    xTaskCreate(vTaskLED, "LED TASK", 1024, &submit, 1, NULL);
}
