#include "ReceiverInput.h"

void setup() {
  Serial1.begin(115200);
}

void loop() {
  for(uint8_t channel = 0; channel < 6; channel++) {
    Serial1.print("CH");
    Serial1.print(channel);
    Serial1.print(": ");
    Serial1.print(ReceiverInput.channels[channel]->duty_cycle);
    Serial1.print(" ");
  }
  Serial1.print("\r\n");
  digitalWrite(13, !digitalRead(13));
  delay(500);
}
