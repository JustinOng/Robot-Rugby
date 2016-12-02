#include "TeeAligner.h"

NewPing Ultrasonic_Left(ULTRASONIC_1_PIN, ULTRASONIC_1_PIN, ULTRASONIC_MAX);
NewPing Ultrasonic_Right(ULTRASONIC_2_PIN, ULTRASONIC_2_PIN, ULTRASONIC_MAX);

TeeAlignerClass::TeeAlignerClass(void) {
  active = 0;
  init();
}

void TeeAlignerClass::init(void) {
  left_distance_running_average = 0;
  right_distance_running_average = 0;
  left_integral = 0;
  right_integral = 0;
}

void TeeAlignerClass::begin(void) {

}

void TeeAlignerClass::loop(void) {
  static int8_t pLeft_power = 0;
  static int8_t pRight_power = 0;

  static float pLeft_distance = 0;
  static float pRight_distance = 0;
  /*if (Ultrasonic_Left.check_timer()) {
    left_distance = (float) Ultrasonic_Left.ping_result / US_ROUNDTRIP_CM;
    Serial1.print("Left Sensor: ");
    Serial1.println(left_distance);
  }
  else if (Ultrasonic_Right.check_timer()) {
    right_distance = (float) Ultrasonic_Right.ping_result / US_ROUNDTRIP_CM;
    Serial1.print("Right Sensor: ");
    Serial1.println(right_distance);
  }
  else */

  if (active && last_ping > PING_INTERVAL) {
    last_ping = 0;

    float ping_distance = (float) Ultrasonic_Left.ping() / US_ROUNDTRIP_CM;

    if (abs((ping_distance - pLeft_distance)*100/pLeft_distance) > ULTRASONIC_DIFFERENCE_THRESHOLD ) {
      left_distance_running_average *= ((float) ULTRASONIC_SAMPLES-1)/ULTRASONIC_SAMPLES;
      left_distance_running_average += ping_distance;
    }

    pLeft_distance = ping_distance;

    ping_distance = (float) Ultrasonic_Right.ping() / US_ROUNDTRIP_CM;

    if (abs((ping_distance - pRight_distance)*100/pRight_distance) > ULTRASONIC_DIFFERENCE_THRESHOLD ) {
      right_distance_running_average *= ((float) ULTRASONIC_SAMPLES-1)/ULTRASONIC_SAMPLES;
      right_distance_running_average += ping_distance;
    }

    pRight_distance = ping_distance;
  }

  if (active && last_motor_set > MOTOR_INTERVAL) {
    last_motor_set = 0;

    float left_distance = left_distance_running_average / ULTRASONIC_SAMPLES;

    float right_distance = right_distance_running_average / ULTRASONIC_SAMPLES;

    Serial1.print("Left: ");
    Serial1.print(left_distance);
    Serial1.print(" cm ");
    Serial1.print("Right: ");
    Serial1.println(right_distance);
    float left_error = LEFT_THRESHOLD - left_distance;
    float right_error = RIGHT_THRESHOLD - right_distance;

    left_integral += left_error;
    right_integral += right_error;

    int8_t rotate = constrain(Receiver.get_channel(1), -MAX_INPUT_POWER, MAX_INPUT_POWER);

    int8_t left_power = constrain(left_error * KP_L + (left_integral * KI_L) - (pLeft_power * KD_L), -MAX_L, MAX_L);
    int8_t right_power = constrain(right_error * KP_R + (right_integral * KI_R) - (pRight_power * KD_R), -MAX_R, MAX_R);

    Motors.set_power(Motors.FrontLeft, left_power + rotate);
    Motors.set_power(Motors.BackLeft, left_power - rotate);
    Motors.set_power(Motors.FrontRight, -right_power + rotate);
    Motors.set_power(Motors.BackRight, -right_power - rotate);
    Serial1.print("Error left: ");
    Serial1.print(left_error);
    Serial1.print(" Power Left: ");
    Serial1.print(left_power);

    Serial1.print(" Error right: ");
    Serial1.print(right_error);
    Serial1.print(" Power Right: ");
    Serial1.println(right_power);

    left_integral *= DECAY_RATE;
    right_integral *= DECAY_RATE;

    pLeft_power = left_power;
    pRight_power = right_power;
  }
}

void TeeAlignerClass::activate(void) {
  if (active == 0) {
    active = 1;

    init();
  }
}

void TeeAlignerClass::deactivate(void) {
  active = 0;
  //Serial1.println("Tee Aligner deactivated");
}

uint8_t TeeAlignerClass::is_active(void) {
  return active > 0;
}

TeeAlignerClass TeeAligner;
