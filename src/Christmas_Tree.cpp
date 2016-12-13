#include "Christmas_Tree.h"

Christmas_Tree_Class::Christmas_Tree_Class(void) {

}

void Christmas_Tree_Class::begin(void) {
  controller = &FastLED.addLeds<NEOPIXEL, STATUS_LEDS_PIN>(leds, NUM_LEDS);
}

void Christmas_Tree_Class::loop(uint8_t front_invert) {
  static elapsedMillis elapsed;
  static elapsedMillis last_blink;
  static uint8_t last_blink_state = 0;

  if (elapsed < THROTTLE_MS) {
    return;
  }

  elapsed = 0;

  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    if (IS_FRONT_LED[i] && !front_invert) {
      leds[i] = COLOR_ACTIVE;
    }
    else {
      if (last_blink > BLINK_INTERVAL) {
        last_blink_state = !last_blink_state;
        last_blink = 0;
      }

      if (last_blink_state) {
        leds[i] = COLOR_NOTACTIVE;
      }
      else {
        leds[i] = CRGB::Black;
      }
    }
  }

  controller->showLeds(BATT_LED_BRIGHTNESS);
}

Christmas_Tree_Class Christmas_Tree;
