#include "Kicker.h"

Encoder winchEncoder(WINCH_ENCODER_A, WINCH_ENCODER_B);

KickerClass::KickerClass(void) {
  // initialise to +1 so that the winch is already inactive
  solenoid_active = WINCH_SOLENOID_PULSE_LENGTH + 1;
  winching = 0;
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

  if (winching == 1) {
    // not in latch area yet, keep winching
    if (analogRead(A0) > KICKER_HALL_EFFECT_THRESHOLD) {
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
  solenoid_active = 0;
}

void KickerClass::reload(void) {

}

uint8_t KickerClass::is_winching(void) {
  return winching > 0;
}

KickerClass Kicker;