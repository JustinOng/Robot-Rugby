#include "ReceiverInput.h"

void setup() {
  Serial1.begin(115200);
}

void loop() {
  for(uint8_t channel = 0; channel < 6; channel++) {
    Serial1.print("CH");
    Serial1.print(channel);
    Serial1.print(": ");
    Serial1.println(ReceiverInput.channels[channel]->duty_cycle);
  }
  delay(500);
}
