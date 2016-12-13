#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include "Kicker.h"
#include "TeeAligner.h"
#include "Current_Monitor.h"
#include "Servo.h"
#include "Encoder.h"
#include "Gyro.h"
#include "Status_Lights.h"
#include "Christmas_Tree.h"

#include "Adafruit_MCP23008.h"

#include "pin_config.h"

//#define PRINT_CONTROLLER_VALUES 1

Adafruit_MCP23008 mcp;
Encoder liftEncoder(LIFT_ENCODER_A, LIFT_ENCODER_B);

Servo gripper;

// minimum value to use when not in competition mode
#define LIFT_MIN_ENCODER_COUNT -5500
// minimum value to use when in competition mode
#define LIFT_MIN_ENCODER_COMP -3300
#define LIFT_HALL_EFFECT_THRESHOLD 800

void setup() {
  Serial1.begin(115200);

  mcp.begin();

  Motors.begin();
  Receiver.begin();
  Kicker.begin();
  //Current_Monitor.begin();
  Gyro.begin();
  Status_Lights.begin();
  Christmas_Tree.begin();

  pinMode(13, OUTPUT);

  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);

  gripper.attach(GRIPPER_SERVO_PIN, 1500, 2100);

  for(uint8_t i = 0; i < 8; i++) {
    mcp.digitalWrite(i, LOW);
    mcp.pinMode(i, OUTPUT);
  }

  pinMode(SERVO_RELAY_PIN, OUTPUT);
  digitalWrite(SERVO_RELAY_PIN, HIGH);

  Serial1.println("Setup finished!");
}

elapsedMillis last_printed;
elapsedMillis last_ping;

elapsedMillis last_servo;

void loop() {
  static float theta_offset = 0;

  Receiver.loop();
  Kicker.loop();
  //Current_Monitor.loop();
  //TeeAligner.loop();
  Gyro.loop();
  Status_Lights.loop(!Receiver.failSafe);
  Christmas_Tree.loop(Receiver.get_channel(9));

  /*Serial1.print("Lift Encoder: ");
  Serial1.println(liftEncoder.read());*/

  /*Serial1.print("Hall A1: ");
  Serial1.println(analogRead(A1));*/

  /*Serial1.print("Hall A0: ");
  Serial1.println(analogRead(A0));*/

  if (last_printed > 500) {
    last_printed = 0;

    #ifdef PRINT_CONTROLLER_VALUES
    for(uint8_t channel = 1; channel <= 10; channel++) {
      Serial1.print("CH");
      Serial1.print(channel);
      Serial1.print(": ");
      //Serial1.print(Receiver.channels[channel-1]);
      Serial1.print(Receiver.get_channel(channel));
        Serial1.print(" ");
    }
    Serial1.print("\r\n");
    #endif // PRINT_CONTROLLER_VALUES
    digitalWrite(13, !digitalRead(13));
  }

  if (Receiver.get_edge(10, ReceiverClass::RISING_EDGE)) {
    Kicker.release();
  }

  if (Receiver.get_edge(7, ReceiverClass::RISING_EDGE)) {
    Kicker.reload();
  }

  if (Receiver.get_channel(7)) {
    if (!Kicker.is_loaded() && Kicker.last_loaded > 500) {
      Kicker.reload();
    }
  }

  for(uint8_t i = 0; i < 8; i++) {
    mcp.digitalWrite(i, Kicker.is_loaded());
  }

  /*if (Receiver.get_channel(6) == 2) {
    TeeAligner.activate();
  }
  else {
    TeeAligner.deactivate();
  }*/

  if (Receiver.get_channel(6) == 0) {
    theta_offset = Gyro.euler[0];
    if (theta_offset < 0) theta_offset += 2*PI;
  }

  if (!TeeAligner.is_active()) {
    int16_t forward = Receiver.get_channel(2);
    int16_t right = Receiver.get_channel(4);

    if (Receiver.get_channel(6) == 2 && Receiver.get_channel(9)) {
      forward *= -1;
      right *= -1;
    }
    else if (Receiver.get_channel(6) == 1) {
      int16_t input_forward = forward;
      int16_t input_right = right;
      float current = Gyro.euler[0];

      if (current < 0) current += 2*PI;

      float correction = current - theta_offset;

      right = cos(correction) * input_right - sin(correction) * input_forward;
      forward = sin(correction) * input_right + cos(correction) * input_forward;
    }

    Motors.set_power(Motors.FrontLeft, forward + right + Receiver.get_channel(1));
    Motors.set_power(Motors.FrontRight, - forward + right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackLeft, forward - right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackRight, - forward - right + Receiver.get_channel(1));
  }

  if (!Kicker.is_winching()) {
    //Motors.set_power(Motors.Winch, 0);
    if (Receiver.get_channel(8) == 1 && abs(Receiver.get_channel(3)) > 50) {
      Motors.set_power(Motors.Winch, Receiver.get_channel(3));
    }
    else {
      Motors.set_power(Motors.Winch, 0);
    }
  }

  if (Receiver.get_channel(8) == 2 && abs(Receiver.get_channel(3)) > 30) {
    if (Receiver.get_channel(3) > 0 && analogRead(LIFT_HALL_EFFECT_PIN) < LIFT_HALL_EFFECT_THRESHOLD) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else if (Receiver.get_channel(3) < 0 && liftEncoder.read() > (Receiver.get_channel(7) ? LIFT_MIN_ENCODER_COMP : LIFT_MIN_ENCODER_COUNT)) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else {
      Motors.set_power(Motors.Lift, 0);
    }

    if (analogRead(LIFT_HALL_EFFECT_PIN) >= LIFT_HALL_EFFECT_THRESHOLD) {
      liftEncoder.write(0);
    }
  }
  else {
    Motors.set_power(Motors.Lift, 0);
  }

  static uint8_t last_state = Receiver.get_channel(5);
  static elapsedMillis gripped_for;

  if (Receiver.failSafe ||  !Receiver.get_channel(5)) {
    if (last_state == 0) {
      gripped_for = 0;
    }

    if (gripped_for > 500) {
      digitalWrite(SERVO_RELAY_PIN, LOW);
    }
    else {
      gripper.write(0);
    }

    last_state = 1;
  }
  else {
    digitalWrite(SERVO_RELAY_PIN, HIGH);
    gripper.write(150);

    last_state = 0;
  }

}
