#ifndef christmas_tree_class_header
#define christmas_tree_class_header

#include <FastLED.h>
#include "pin_config.h"

// time in ms to delay execution of every loop by
#define THROTTLE_MS 100

#define BATT_LED_BRIGHTNESS 100

#define NUM_LEDS 10

#define BLINK_INTERVAL 100

class Christmas_Tree_Class {
  public:
    Christmas_Tree_Class(void);
    void begin(void);
    void loop(uint8_t front_invert);
  private:
    CRGB leds[NUM_LEDS];
    CLEDController * controller;

    uint8_t IS_FRONT_LED[NUM_LEDS] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

    CRGB COLOR_ACTIVE = CRGB::Green;
    CRGB COLOR_NOTACTIVE = CRGB::Red;
};

extern Christmas_Tree_Class Christmas_Tree;

#endif
