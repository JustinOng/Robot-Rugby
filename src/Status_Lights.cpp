#include "Status_Lights.h"
Status_Lights_Class::Status_Lights_Class(void) {

}

void Status_Lights_Class::begin(void) {
  controller = &FastLED.addLeds<NEOPIXEL, BATT_LEDS_PIN>(leds, NUM_LEDS);
}

void Status_Lights_Class::loop(uint8_t connected) {
  static elapsedMillis elapsed;

  if (elapsed < THROTTLE_MS) {
    return;
  }

  elapsed = 0;
  
  leds[0] = connected ? COLOR_CONNECTED : COLOR_DISCONNECTED;

  controller->showLeds(BATT_LED_BRIGHTNESS);
}

Status_Lights_Class Status_Lights;
