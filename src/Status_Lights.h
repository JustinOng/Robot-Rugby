#ifndef status_light_header
#define status_light_header

#include <FastLED.h>
#include "pin_config.h"

// time in ms to delay execution of every loop by
#define THROTTLE_MS 100

#define BATT_LED_BRIGHTNESS 20

#define NUM_LEDS 3

class Status_Lights_Class {
  public:
    Status_Lights_Class(void);
    void begin(void);
    void loop(uint8_t connected);
  private:
    CRGB leds[NUM_LEDS];
    CLEDController * controller;

    CRGB COLOR_CONNECTED = CRGB::Green;
    CRGB COLOR_DISCONNECTED = CRGB::Red;
};

extern Status_Lights_Class Status_Lights;

#endif
