#include "Motors.h"

MotorsClass::MotorsClass(void) {
  
}

void MotorsClass::set_power(Motor motor, int8_t power) {
  switch(motor) {
    case FrontLeft:
      
      break;
    case FrontRight:
      
      break;
    case Backleft:
      
      break;
    case BackRight:
      
      break;
  }
}

MotorsClass Motors;
