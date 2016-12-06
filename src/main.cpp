#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include "Battery_Monitor.h"
#include "Kicker.h"
#include "TeeAligner.h"
#include "Current_Monitor.h"
#include "Servo.h"
#include "Encoder.h"

#include "Adafruit_MCP23008.h"

#include "pin_config.h"

//#define PRINT_CONTROLLER_VALUES 1

Adafruit_MCP23008 mcp;
Encoder liftEncoder(LIFT_ENCODER_A, LIFT_ENCODER_B);

Servo gripper;

void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  Battery_Monitor.begin();
  Kicker.begin();
  Current_Monitor.begin();

  pinMode(13, OUTPUT);

  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);

  mcp.begin();

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
  Battery_Monitor.loop();
  Receiver.loop();
  Kicker.loop();
  Current_Monitor.loop();
  //TeeAligner.loop();

  Serial1.print("Lift Encoder: ");
  Serial1.println(liftEncoder.read());

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

  if (!TeeAligner.is_active()) {
    int16_t forward = Receiver.get_channel(2);
    int16_t right = Receiver.get_channel(4);

    if (Receiver.get_channel(9)) {
      forward *= -1;
      right *= -1;
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
    if (Receiver.get_channel(3) > 0 && analogRead(LIFT_HALL_EFFECT_PIN) < 800) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else if (Receiver.get_channel(3) < 0 && liftEncoder.read() > -5000) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else {
      Motors.set_power(Motors.Lift, 0);
    }

    if (analogRead(LIFT_HALL_EFFECT_PIN) >= 800) {
      liftEncoder.write(0);
    }
  }
  else {
    Motors.set_power(Motors.Lift, 0);
  }

  if (Receiver.get_channel(8) == 0) {
    uint8_t angle = map(Receiver.get_channel(3), 100, -100, 180, 0);
    gripper.write(angle);
    Serial1.print(angle);
    Serial1.println(" degrees");
  }
  else {
    if (Receiver.get_channel(5)) {
      gripper.write(26);
      //gripper.detach();
    }
    else {
      //gripper.attach(GRIPPER_SERVO_PIN, 1500, 2100);
      gripper.write(130);
    }
  }

}
