#ifndef motor_included
#define motor_included

#include <Arduino.h>

class MotorsClass {
  private:

  public:
    enum Motor {
      FrontLeft,
      FrontRight,
      BackLeft,
      BackRight
    };
    
    MotorsClass(void);
    void set_power(Motor motor, int8_t power);
};

extern MotorsClass Motors;

#endif
