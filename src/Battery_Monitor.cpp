#include "Battery_Monitor.h"

Battery_Monitor_Class::Battery_Monitor_Class(void) {

}

void Battery_Monitor_Class::begin(void) {
  controller = &FastLED.addLeds<NEOPIXEL, BATT_LEDS_PIN>(leds, NUM_LEDS);
}

void Battery_Monitor_Class::loop(void) {
  static elapsedMillis elapsed;

  if (elapsed < THROTTLE_MS) {
    return;
  }

  elapsed = 0;

  for(uint8_t pin = 0; pin < NUM_CELL_PINS; pin++) {
    uint16_t adc_value = analogRead(CELL_PINS[pin]);

    if (pin < (NUM_CELL_PINS-1)) {
      // subtract the next pin value because the balance leads are in series
      adc_value -= analogRead(CELL_PINS[pin+1]);
    }

    float voltage = (float) (adc_value)/ 1024 * 3.3;

    // scale up because of voltage divider
    voltage *= (float) (VOLTAGE_DIVIDER_MAIN+VOLTAGE_DIVIDER_SIDE)/VOLTAGE_DIVIDER_SIDE;

    if (voltage < LEVEL_ERROR) {
      leds[LED_INDEX[pin]] = COLOR_ERROR;
    }
    else if (voltage < LEVEL_WARN) {
      leds[LED_INDEX[pin]] = COLOR_WARN;
    }
    else {
      leds[LED_INDEX[pin]] = COLOR_OK;
    }
  }

  controller->showLeds(BATT_LED_BRIGHTNESS);
}

Battery_Monitor_Class Battery_Monitor;
