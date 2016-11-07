#ifndef motor_included
#define motor_included

#include <Arduino.h>

class MotorsClass {
  private:

  public:
    enum Motor {
      FrontLeft = 0,
      FrontRight,
      BackLeft,
      BackRight,
      Lift,
      Winch,
      NUM_MOTORS
    };

    MotorsClass(void);
    void begin(void);
    void set_power(Motor motor, int16_t power);
};

extern MotorsClass Motors;

#endif
