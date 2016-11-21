#ifndef battery_monitor_class_h
#define battery_monitor_class_h

#include <FastLED.h>

// time in ms to delay execution of every loop by
#define THROTTLE_MS 1000

#define LEVEL_WARN 3.7
#define LEVEL_ERROR 3.4

#define BATT_LED_BRIGHTNESS 20

#define BATT_LEDS_PIN 17
#define NUM_LEDS 3

#define SAMPLES 10

/*
  define the voltage divider that scales down the maximum voltage from the
  MAIN is the value of the resistor that is connected between Ax and GND
  SIDE is the value of the resistor that is connected between Vcc and Ax
*/
#define VOLTAGE_DIVIDER_MAIN 51
#define VOLTAGE_DIVIDER_SIDE 18

#define NUM_CELL_PINS 3

class Battery_Monitor_Class {
  public:
    Battery_Monitor_Class(void);
    void begin(void);
    void loop(void);
  private:
    // arranged in order from highest to lowest voltage
    const uint8_t CELL_PINS[NUM_CELL_PINS] = { 23, 22, 21 };
    // which index in the strip to display the status of the corresponding pin above
    const uint8_t LED_INDEX[NUM_CELL_PINS] = { 0, 1, 2 };

    uint16_t readingAverage[NUM_CELL_PINS] = {0};

    CRGB leds[NUM_LEDS];
    CLEDController * controller;

    CRGB COLOR_OK = CRGB::Green;
    CRGB COLOR_WARN = CRGB::Orange;
    CRGB COLOR_ERROR = CRGB::Red;
};

extern Battery_Monitor_Class Battery_Monitor;

#endif
