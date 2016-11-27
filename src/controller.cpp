#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include "Battery_Monitor.h"
#include "Kicker.h"

#include "Adafruit_MCP23008.h"
#include "NewPing.h"

#include "pin_config.h"

#define ULTRASONIC_MAX 50

//#define PRINT_CONTROLLER_VALUES 1

Adafruit_MCP23008 mcp;

NewPing sonar1(ULTRASONIC_1_PIN, ULTRASONIC_1_PIN, ULTRASONIC_MAX);
NewPing sonar2(ULTRASONIC_2_PIN, ULTRASONIC_2_PIN, ULTRASONIC_MAX);

void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  Battery_Monitor.begin();
  Kicker.begin();

  pinMode(GRIPPER_SOLENOID_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(24, OUTPUT);
  digitalWrite(24, HIGH);

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
  static float theta = 0;
  static float theta_offset = 0;

  static uint8_t winching = 0;

  static elapsedMillis gripping_start;
  static uint8_t gripping = 0;

  Battery_Monitor.loop();
  Receiver.loop();

  /*Serial1.print("Encoder: ");
  Serial1.println(winchEncoder.read());*/

  /*Serial1.print("Hall A1: ");
  Serial1.println(analogRead(15));*/

  /*Serial1.print("Hall A): ");
  Serial1.println(analogRead(A0));*/

  if (last_ping > 100) {
    last_ping = 0;
    Serial1.print("Ping 1: ");
    Serial1.print((float) sonar1.ping() / US_ROUNDTRIP_CM);
    Serial1.println("cm");
    Serial1.print("Ping 2: ");
    Serial1.print((float) sonar2.ping() / US_ROUNDTRIP_CM);
    Serial1.println("cm");
  }

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
    winching = 1;
  }

  if (Receiver.get_channel(6) == 0) {
    for(uint8_t i = 0; i < 8; i++) {
      mcp.digitalWrite(i, LOW);
    }
  }
  else {
    for(uint8_t i = 0; i < 8; i++) {
      mcp.digitalWrite(i, HIGH);
    }
  }

  theta_offset = Receiver.get_channel(9) ? 180 : 0;

  digitalWrite(GRIPPER_SOLENOID_PIN, Receiver.get_channel(5));

  float theta_corrected = theta - theta_offset;

  if (theta_corrected < 0) theta_corrected += 360;

  float theta_rads = theta_corrected * 71 / 4068;

  int16_t forward = Receiver.get_channel(2);
  int16_t right = Receiver.get_channel(4);

  int16_t new_right = cos(theta_rads) * right - sin(theta_rads) * forward;
  int16_t new_forward = sin(theta_rads) * right + cos(theta_rads) * forward;

  Motors.set_power(Motors.FrontLeft, new_forward + new_right + Receiver.get_channel(1));
  Motors.set_power(Motors.FrontRight, - new_forward + new_right + Receiver.get_channel(1));
  Motors.set_power(Motors.BackLeft, new_forward - new_right + Receiver.get_channel(1));
  Motors.set_power(Motors.BackRight, - new_forward - new_right + Receiver.get_channel(1));

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
    if (Receiver.get_channel(3) > 0 && analogRead(A1) > 480) {
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
