#include "TeeAligner.h"

NewPing Ultrasonic_Left(ULTRASONIC_1_PIN, ULTRASONIC_1_PIN, ULTRASONIC_MAX);
NewPing Ultrasonic_Right(ULTRASONIC_2_PIN, ULTRASONIC_2_PIN, ULTRASONIC_MAX);

TeeAlignerClass::TeeAlignerClass(void) {
  active = 0;
  left_distance = 0;
  right_distance = 0;
}

void TeeAlignerClass::begin(void) {

}

void TeeAlignerClass::loop(void) {
  if (Ultrasonic_Left.check_timer()) {
    left_distance = (float) Ultrasonic_Left.ping_result / US_ROUNDTRIP_CM;
  }
  else if (Ultrasonic_Right.check_timer()) {
    right_distance = (float) Ultrasonic_Right.ping_result / US_ROUNDTRIP_CM;
  }
  else if (last_ping > 50) {
    last_ping = 0;

    Ultrasonic_Left.ping_timer();
    Ultrasonic_Right.ping_timer();
  }
}

void TeeAlignerClass::activate(void) {
  active = 1;
  Serial1.println("Tee Aligner activated");
}

void TeeAlignerClass::deactivate(void) {
  active = 0;
  Serial1.println("Tee Aligner deactivated");
}

uint8_t TeeAlignerClass::is_active(void) {
  return active > 0;
}

TeeAlignerClass TeeAligner;
