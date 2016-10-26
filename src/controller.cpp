#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include <Encoder.h>

#define SOLENOID_PULSE_LENGTH 100
#define SOLENOID_PIN 6

#define PRINT_CONTROLLER_VALUES 1

elapsedMillis solenoid_active;
void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  pinMode(SOLENOID_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  solenoid_active = SOLENOID_PULSE_LENGTH + 1;

  Serial1.println("Setup finished!");
}

elapsedMillis last_printed;
elapsedMillis last_motor_set;
Encoder winchEncoder(11, 12);

void loop() {
  static float theta = 0;
  static float theta_offset = 0;
  static uint8_t pCH9 = 2;
  static uint8_t pCH10 = 2;
  uint8_t CH9 = Receiver.get_channel(9);
  uint8_t CH10 = Receiver.get_channel(10);

  static uint8_t winching = 0;

  Receiver.loop();

  /*Serial1.print("Encoder: ");
  Serial1.println(winchEncoder.read());*/

  if (last_printed > 100) {
    last_printed = 0;

    #ifdef PRINT_CONTROLLER_VALUES
    Serial1.print("F: ");
    Serial1.println(Receiver.failSafe);
    for(uint8_t channel = 1; channel <= 16; channel++) {
      Serial1.print("CH");
      Serial1.print(channel);
      Serial1.print(": ");
      //Serial1.print(Receiver.channels[channel].duty_cycle);
      Serial1.print(Receiver.get_channel(channel));
        Serial1.print(" ");
    }
    Serial1.print("\r\n");
    #endif // PRINT_CONTROLLER_VALUES
    digitalWrite(13, !digitalRead(13));
  }

  if (solenoid_active < SOLENOID_PULSE_LENGTH) {
    digitalWrite(SOLENOID_PIN, HIGH);
  }
  else {
    digitalWrite(SOLENOID_PIN, LOW);
  }

  if (CH9 == 1 && pCH9 == 0) {
    // if switch was just pulled down

    theta_offset = theta;
    solenoid_active = 0;
  }

  if (CH10 == 1 && pCH10 == 0) {
    winching = 1;
  }

  if (last_motor_set > 50) {
    float theta_corrected = theta - theta_offset;

    if (theta_corrected < 0) theta_corrected += 360;

    float theta_rads = theta_corrected * 71 / 4068;
    last_motor_set = 0;

    int16_t forward = Receiver.get_channel(2);
    int16_t right = Receiver.get_channel(4);

    int16_t new_right = cos(theta_rads) * right - sin(theta_rads) * forward;
    int16_t new_forward = sin(theta_rads) * right + cos(theta_rads) * forward;

    Motors.set_power(Motors.FrontLeft, new_forward + new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.FrontRight, - new_forward + new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackLeft, new_forward - new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackRight, - new_forward - new_right + Receiver.get_channel(1));

    if (winching) {
      if (winching == 1) {
        // not in latch area yet, keep winching
        if (analogRead(A0) > 280) {
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
        if (winchEncoder.read() < 9000) {
          Motors.set_power(Motors.Winch, 100);
        }
        else {
          Motors.set_power(Motors.Winch, 0);
          winching = 0;
        }
      }
    }
    else {
      if (abs(Receiver.get_channel(3)) > 50) {
        Motors.set_power(Motors.Winch, Receiver.get_channel(3));
      }
      else {
        Motors.set_power(Motors.Winch, 0);
      }
    }
  }

  pCH9 = CH9;
  pCH10 = CH10;
}
