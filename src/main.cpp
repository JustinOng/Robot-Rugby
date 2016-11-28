#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include "Battery_Monitor.h"
#include "Kicker.h"
#include "TeeAligner.h"
#include "Current_Monitor.h"

#include "Adafruit_MCP23008.h"

#include "pin_config.h"

//#define PRINT_CONTROLLER_VALUES 1

Adafruit_MCP23008 mcp;

void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  Battery_Monitor.begin();
  Kicker.begin();
  Current_Monitor.begin();

  pinMode(GRIPPER_SOLENOID_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(MOTOR_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTOR_ENABLE_PIN, HIGH);

  mcp.begin();

  for(uint8_t i = 0; i < 8; i++) {
    mcp.digitalWrite(i, LOW);
    mcp.pinMode(i, OUTPUT);
  }

  Serial1.println("Setup finished!");
}

elapsedMillis last_printed;
elapsedMillis last_ping;

void loop() {
  Battery_Monitor.loop();
  Receiver.loop();
  Kicker.loop();
  Current_Monitor.loop();

  /*Serial1.print("Encoder: ");
  Serial1.println(winchEncoder.read());*/

  /*Serial1.print("Hall A1: ");
  Serial1.println(analogRead(15));*/

  /*Serial1.print("Hall A): ");
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

  if (Receiver.get_edge(7, ReceiverClass::FALLING_EDGE)) {
    Kicker.reload();
  }

  for(uint8_t i = 0; i < 8; i++) {
    mcp.digitalWrite(i, Kicker.is_loaded());
  }

  digitalWrite(GRIPPER_SOLENOID_PIN, Receiver.get_channel(5));

  if (Receiver.get_channel(6)) {
    TeeAligner.activate();
  }
  else {
    TeeAligner.deactivate();
  }

  if (!TeeAligner.is_active()) {
    int16_t forward = Receiver.get_channel(2);
    int16_t right = Receiver.get_channel(4);

    if (Receiver.get_channel(9)) {
      forward *= -1;
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

  if (Receiver.get_channel(8) == 2 && abs(Receiver.get_channel(3)) > 50) {
    if (Receiver.get_channel(3) > 0 && analogRead(LIFT_HALL_EFFECT_PIN) > 480) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else if (Receiver.get_channel(3) < 0) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else {
      Motors.set_power(Motors.Lift, 0);
    }
  }
  else {
    Motors.set_power(Motors.Lift, 0);
  }
}
