#include "Kicker.h"

Encoder winchEncoder(WINCH_ENCODER_A, WINCH_ENCODER_B);

KickerClass::KickerClass(void) {
  // initialise to +1 so that the winch is already inactive
  solenoid_active = WINCH_SOLENOID_PULSE_LENGTH + 1;
  winching = 0;
  loaded = 1;
}

void KickerClass::begin(void) {
  pinMode(WINCH_SOLENOID_PIN, OUTPUT);
}

void KickerClass::loop(void) {
  if (solenoid_active < WINCH_SOLENOID_PULSE_LENGTH) {
    digitalWrite(WINCH_SOLENOID_PIN, HIGH);
  }
  else {
    digitalWrite(WINCH_SOLENOID_PIN, LOW);
  }

  uint16_t kicker_hall_effect_position = analogRead(KICKER_HALL_EFFECT_PIN);

  if (kicker_hall_effect_position < KICKER_HALL_EFFECT_THRESHOLD) {
    last_loaded = 0;
    loaded = 1;
  }
  else if (kicker_hall_effect_position > (KICKER_HALL_EFFECT_THRESHOLD + KICKER_HALL_EFFECT_HYSTERESIS)) {
    loaded = 0;
  }

  if (winching == 1) {
    // not in latch area yet, keep winching
    if (last_loaded > 0) {
      Motors.set_power(Motors.Winch, -100);
    }
    else {
      // has reached threshold, stop motors, reset encoder
      Motors.set_power(Motors.Winch, 0);
      winching = 2;
      winchEncoder.write(0);
    }
  }
  else if (winching == 2) {
    if (winchEncoder.read() < KICKER_RELEASE_COUNT) {
      Motors.set_power(Motors.Winch, 100);
    }
    else {
      Motors.set_power(Motors.Winch, 0);
      winching = 0;
    }
  }
}

void KickerClass::release(void) {
  if (!is_loaded() && solenoid_active < 1000) {
    // dont release if active in last second and not loaded
    return;
  }

  solenoid_active = 0;
}

void KickerClass::reload(void) {
  if (is_loaded()) return;

  winching = 1;
}

uint8_t KickerClass::is_winching(void) {
  return winching > 0;
}

uint8_t KickerClass::is_loaded(void) {
  return loaded;
}

KickerClass Kicker;
