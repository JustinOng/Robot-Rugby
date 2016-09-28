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
      NUM_MOTORS
    };

    const uint8_t pwm_pins[4] = {21, 22, 20, 23};
    const uint8_t dir_pins[4] = {10, 11, 9, 12};
    
    MotorsClass(void);
    void begin(void);
    void set_power(Motor motor, int16_t power);
};

extern MotorsClass Motors;

#endif
