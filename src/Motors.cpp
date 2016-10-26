#include "Motors.h"

MotorsClass::MotorsClass(void) {
  
}

void MotorsClass::begin(void) {
  Serial3.begin(115200);
}

void MotorsClass::set_power(Motor motor, int16_t power) {
  Serial3.print("M");
  Serial3.print(motor+1);
  Serial3.print("D");
  Serial3.print(power > 0 ? 1:0);
  Serial3.print("P");
  Serial3.println(abs(power));
}

MotorsClass Motors;
