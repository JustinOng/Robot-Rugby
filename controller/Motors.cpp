#include "Motors.h"

MotorsClass::MotorsClass(void) {
  
}

void MotorsClass::begin(void) {
  for(uint8_t i = 0; i < NUM_MOTORS; i++) {
    pinMode(pwm_pins[i], OUTPUT);
    pinMode(dir_pins[i], OUTPUT);
  }

  analogWriteFrequency(pwm_pins[0], 20000);
}

void MotorsClass::set_power(Motor motor, int16_t power) {
  analogWrite(pwm_pins[motor], 255 - min(map(abs(power), 0, 127, 0, 255), 255));
  digitalWriteFast(dir_pins[motor], power > 0 ? 1 : 0);
}

MotorsClass Motors;
