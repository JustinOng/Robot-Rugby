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
      Winch,
      Lift,
      NUM_MOTORS
    };

    int8_t current_motor_power[NUM_MOTORS] = {0};

    MotorsClass(void);
    void begin(void);
    void set_power(Motor motor, int16_t power);
  private:
    elapsedMillis lastChanged[NUM_MOTORS];
};

extern MotorsClass Motors;

#endif
