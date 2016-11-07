#include <Arduino.h>

#include "Receiver.h"
#include "Motors.h"
#include <Encoder.h>

#include <FastLED.h>

#define BATT_LIGHT_PIN 17

#define WINCH_SOLENOID_PULSE_LENGTH 50
#define WINCH_SOLENOID_PIN 6

#define GRIPPER_SOLENOID_PIN 5

#define PRINT_CONTROLLER_VALUES 1

CRGB BATT_LEDs[3];
CRGB STATUS_LEDs[2];

elapsedMillis solenoid_active;
void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  pinMode(WINCH_SOLENOID_PIN, OUTPUT);
  pinMode(GRIPPER_SOLENOID_PIN, OUTPUT);
  pinMode(13, OUTPUT);

  solenoid_active = WINCH_SOLENOID_PULSE_LENGTH + 1;

  //FastLED.addLeds<NEOPIXEL, BATT_LIGHT_PIN>(BATT_LEDs, 3);

  FastLED.addLeds<NEOPIXEL, 16>(STATUS_LEDs, 2);
  //FastLED.setBrightness(20);

  //BATT_LEDs[0] = CRGB::Red; FastLED.show();
  STATUS_LEDs[0] = CRGB::Blue;
  STATUS_LEDs[1] = CRGB::Blue;
  FastLED.show();

  Serial1.println("Setup finished!");
}

elapsedMillis last_printed;
elapsedMillis last_motor_set;
Encoder winchEncoder(11, 12);

void loop() {
  static float theta = 0;
  static float theta_offset = 0;
  static uint8_t pCH7 = 2;
  static uint8_t pCH10 = 2;
  static uint8_t pCH5 = 2;
  uint8_t CH7 = Receiver.get_channel(7);
  uint8_t CH10 = Receiver.get_channel(10);
  uint8_t CH5 = Receiver.get_channel(5);

  static uint8_t winching = 0;

  static elapsedMillis gripping_start;
  static uint8_t gripping = 0;

  Receiver.loop();

  /*Serial1.print("Encoder: ");
  Serial1.println(winchEncoder.read());*/

  if (last_printed > 100) {
    last_printed = 0;

    #ifdef PRINT_CONTROLLER_VALUES
    for(uint8_t channel = 1; channel <= 10; channel++) {
      Serial1.print("CH");
      Serial1.print(channel);
      Serial1.print(": ");
      Serial1.print(Receiver.channels[channel-1]);
      //Serial1.print(Receiver.get_channel(channel));
        Serial1.print(" ");
    }
    Serial1.print("\r\n");
    #endif // PRINT_CONTROLLER_VALUES
    digitalWrite(13, !digitalRead(13));
  }

  if (solenoid_active < WINCH_SOLENOID_PULSE_LENGTH) {
    digitalWrite(WINCH_SOLENOID_PIN, HIGH);
  }
  else {
    digitalWrite(WINCH_SOLENOID_PIN, LOW);
  }

  if (CH10 && pCH10 == 0) {
    // if switch was just pulled down

    //theta_offset = theta;
    solenoid_active = 0;
  }

  if (CH7 && pCH7 == 0) {
    winching = 1;
  }

  theta_offset = Receiver.get_channel(9) ? 180 : 0;

  /*if (CH5 == 1 && pCH5 == 0) {
    gripping = 1;
    gripping_start = 0;
  }*/

  digitalWrite(GRIPPER_SOLENOID_PIN, Receiver.get_channel(5));

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
        if (winchEncoder.read() < 7000) {
          Motors.set_power(Motors.Winch, 100);
        }
        else {
          Motors.set_power(Motors.Winch, 0);
          winching = 0;
        }
      }
    }
    else {
      //Motors.set_power(Motors.Winch, 0);
      if (Receiver.get_channel(8) == 1 && abs(Receiver.get_channel(3)) > 50) {
        Motors.set_power(Motors.Winch, Receiver.get_channel(3));
      }
      else {
        Motors.set_power(Motors.Winch, 0);
      }
    }

    if (Receiver.get_channel(8) == 2 && abs(Receiver.get_channel(3)) > 50) {
      Motors.set_power(Motors.Lift, Receiver.get_channel(3));
    }
    else {
      Motors.set_power(Motors.Lift, 0);
    }
  }

  pCH7 = CH7;
  pCH10 = CH10;
  pCH5 = CH5;
}
