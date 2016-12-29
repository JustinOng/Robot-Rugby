#include "Christmas_Tree.h"

Christmas_Tree_Class::Christmas_Tree_Class(void) {

}

void Christmas_Tree_Class::begin(void) {
  controller = &FastLED.addLeds<NEOPIXEL, STATUS_LEDS_PIN>(leds, NUM_LEDS);
  for(uint8_t i = 0; i < NUM_LEDS; i++) {
    led_value[i] = random(255);
  }
}

void Christmas_Tree_Class::loop(uint8_t active, uint8_t front_invert) {
  static elapsedMillis elapsed;
  static elapsedMillis last_blink;
  static uint8_t last_blink_state = 0;

  if (elapsed < THROTTLE_MS) {
    return;
  }

  elapsed = 0;

  if (active) {
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
  }
  else {
    for(uint8_t i = 0; i < NUM_LEDS; i++) {
      leds[i] = Wheel(led_value[i]++);
    }
  }

  controller->showLeds(LED_BRIGHTNESS);
}

// function taken from adafruit neopixel strandtest example
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
CRGB Christmas_Tree_Class::Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
}

Christmas_Tree_Class Christmas_Tree;
