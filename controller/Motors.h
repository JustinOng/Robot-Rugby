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

    const uint8_t pwm_pins[4] = {23, 22, 21, 20};
    const uint8_t dir_pins[4] = {12, 11, 10, 9};
    
    MotorsClass(void);
    void begin(void);
    void set_power(Motor motor, int16_t power);
};

extern MotorsClass Motors;

#endif
