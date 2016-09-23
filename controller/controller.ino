#include "Receiver.h"
#include "Motors.h"

void setup() {
  Serial1.begin(115200);

  Motors.begin();
  Receiver.begin();
  pinMode(13, OUTPUT);
}

elapsedMillis last_printed;
elapsedMillis last_motor_set;
void loop() {
  if (last_printed > 100) {
    last_printed = 0;
    for(uint8_t channel = 1; channel <= 6; channel++) {
      Serial1.print("CH");
      Serial1.print(channel);
      Serial1.print(": ");
      //Serial1.print(Receiver.channels[channel].duty_cycle);
      Serial1.print(Receiver.get_channel(channel));
      Serial1.print(" ");
    }
    Serial1.print("\r\n");
    digitalWrite(13, !digitalRead(13));
  }

  if (last_motor_set > 50) {
    last_motor_set = 0;
    //if (abs(Receiver.get_channel(2)) > 5) {
      Motors.set_power(Motors.FrontRight, Receiver.get_channel(2));
    //}
  }
}
