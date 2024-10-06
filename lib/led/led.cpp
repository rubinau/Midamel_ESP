#include <Arduino.h>
#include <led.h>

// Constructor to initialize the RGB pins and active high/low setting
LedRGB::LedRGB(int bPin, int rPin, int gPin, bool isActiveHigh, int kelaz)
    : redPin(rPin), greenPin(gPin), bluePin(bPin), activeHigh(isActiveHigh)
{
    redPin = rPin;
    bluePin = bPin;
    greenPin = gPin;
    activeHigh = isActiveHigh;
    kelas = kelaz;
    kelasA = 0;
    kelasB = 0;
    kelasC = 0;
    kelasD = 0;
}

// Private method to write to a pin based on activeHigh setting

void LedRGB::setupLed()
{
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    // Initialize all pins to LOW (off)
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
}

void LedRGB::writePin(int pin, bool state)
{
    digitalWrite(pin, activeHigh ? state : !state);
}

// Function to enable the red component
void LedRGB::enableRed(bool state)
{
    writePin(redPin, state);
}

// Function to enable the green component
void LedRGB::enableGreen(bool state)
{
    writePin(greenPin, state);
}

// Function to enable the blue component
void LedRGB::enableBlue(bool state)
{
    writePin(bluePin, state);
}

// Function to disable all components
void LedRGB::disableAll()
{
    writePin(redPin, LOW);
    writePin(greenPin, LOW);
    writePin(bluePin, LOW);
}

void LedRGB::updateState(int count)
{
    switch (count % 4)
    {
    case 0:
        disableAll(); // Null state, all LEDs off
        break;

    case 3:
        enableRed(false);
        // enableGreen(true); // Green state
        callBlinkLed(greenPin, 5, 800, activeHigh);
        enableBlue(false);
        break;
    case 2:
        enableRed(false);
        enableGreen(false);
        // enableBlue(true); // Blue state
        callBlinkLed(greenPin,redPin, 5,800,activeHigh);
        break;
    case 1:
        // enableRed(true); // Red state
        callBlinkLed(redPin, 5, 800, activeHigh);
        enableGreen(false);
        enableBlue(false);
        break;
    default:
        break;
    }
}

#define UPDATE_IF_CHANGE 1
void LedRGB::updateFromString(int count_tk, int count_sd, int count_smp, int count_sma)
{
    bool update = UPDATE_IF_CHANGE;
    switch (kelas)
    {
    case 0:
        if (count_tk != kelasA)
        {
            updateState(count_tk);
        }
        break;
    case 1:
        if (count_sd != kelasB)
        {
            updateState(count_sd);
        }
        break;
    case 2:
        if (count_smp != kelasC)
        {
            updateState(count_smp);
        }
        break;
    case 3:
        if (count_sma != kelasD)
        {
            updateState(count_sma);
        }
        break;
    default:
        break;
    }
    kelasA = count_tk;
    kelasB = count_sd;
    kelasC = count_smp;
    kelasD = count_sma;
}

void blinkLED(int led, int times, int delay_time, bool active_high)
{
    for (int i = 0; i < times; i++)
    {
        // Serial.printf("Blinking %d\n", led);
        // Serial.println("Blinking");

        digitalWrite(led, active_high);
        vTaskDelay(delay_time);
        digitalWrite(led, !active_high);
        vTaskDelay(delay_time);
    }
    digitalWrite(led, active_high);
    vTaskDelay(3000);
    digitalWrite(led, !active_high);
}

void blinkLED(int led_1, int led_2, int times, int delay_time, bool active_high){
    for (int i = 0; i < times; i++)
    {
        // Serial.printf("Blinking %d\n", led);
        // Serial.println("Blinking");

        digitalWrite(led_1, active_high);
        digitalWrite(led_2, active_high);
        vTaskDelay(delay_time);
        digitalWrite(led_1, !active_high);
        digitalWrite(led_2, !active_high);
        vTaskDelay(delay_time);
    }
    digitalWrite(led_1, active_high);
    digitalWrite(led_2, active_high);
    vTaskDelay(3000);
    digitalWrite(led_1, !active_high);
    digitalWrite(led_2, !active_high);
}

void vTaskLED(void *parameter)
{
    led_param params = *((led_param *)parameter);
    if (params.pin2 == -99){
        blinkLED(params.pin1, params.times, params.delay_time, params.active_high);
    } else {
        blinkLED(params.pin1, params.pin2, params.times, params.delay_time, params.active_high);
    }
    free(parameter);
    vTaskDelete(NULL);
}

void callBlinkLed(int led, int times, int delay_time, bool active_high)
{
    led_param *submit = (led_param *)malloc(sizeof(led_param));
    if (submit == NULL)
    {
        Serial.println("Failed to allocate memory for LED task parameter");
        return;
    }
    submit->pin1 = led;
    submit->pin2 = -99;
    submit->times = times;
    submit->delay_time = delay_time;
    submit->active_high = active_high;
    TaskHandle_t taskHandle;
    xTaskCreate(vTaskLED, "LED TASK", 4096, submit, 1, &taskHandle);

    if (taskHandle != NULL)
    {
        Serial.printf("LED TASK high water mark: %u\n", uxTaskGetStackHighWaterMark(taskHandle));
    }
}

void callBlinkLed(int led1,int led2, int times, int delay_time, bool active_high)
{
    led_param *submit = (led_param *)malloc(sizeof(led_param));
    if (submit == NULL)
    {
        Serial.println("Failed to allocate memory for LED task parameter");
        return;
    }
    submit->pin1 = led1;
    submit->pin2 = led2;
    submit->times = times;
    submit->delay_time = delay_time;
    submit->active_high = active_high;
    TaskHandle_t taskHandle;
    xTaskCreate(vTaskLED, "LED TASK", 4096, submit, 1, &taskHandle);

    if (taskHandle != NULL)
    {
        Serial.printf("LED TASK high water mark: %u\n", uxTaskGetStackHighWaterMark(taskHandle));
    }
}
