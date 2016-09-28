#include "Receiver.h"
#include "Motors.h"

void setup() {
  Serial1.begin(115200);
  Serial3.begin(115200);
  Serial3.setTimeout(5);

  Motors.begin();
  Receiver.begin();
  pinMode(13, OUTPUT);

  Serial1.println("Setup finished!");
}

elapsedMillis last_printed;
elapsedMillis last_motor_set;
void loop() {
  static float theta = 0;
  static float theta_offset = 0;
  static uint8_t pCH5 = 2;
  
  if (last_printed > 100) {
    last_printed = 0;
    /*for(uint8_t channel = 1; channel <= 6; channel++) {
      Serial1.print("CH");
      Serial1.print(channel);
      Serial1.print(": ");
      //Serial1.print(Receiver.channels[channel].duty_cycle);
      Serial1.print(Receiver.get_channel(channel));
        Serial1.print(" ");
    }
    Serial1.print("\r\n");*/
    digitalWrite(13, !digitalRead(13));
  }

  if (last_motor_set > 50) {
    float theta_rads = theta * 71 / 4068;
    last_motor_set = 0;

    int16_t forward = Receiver.get_channel(4);
    int16_t right = Receiver.get_channel(2);

    int16_t new_right = cos(theta_rads) * right - sin(theta_rads) * forward;
    int16_t new_forward = sin(theta_rads) * right + cos(theta_rads) * forward;
    
    Motors.set_power(Motors.FrontLeft, new_forward + new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.FrontRight, - new_forward + new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackLeft, new_forward - new_right + Receiver.get_channel(1));
    Motors.set_power(Motors.BackRight, - new_forward - new_right + Receiver.get_channel(1));
  }
  
  if (Serial3.findUntil("Headings  ", "\n\r")) {
    theta = Serial3.parseFloat();
  }
}
