#ifndef kickerIncluded
#define kickerIncluded

#include "Arduino.h"
#include <Encoder.h>
#include "pin_config.h"

#include "Motors.h"

#define WINCH_SOLENOID_PULSE_LENGTH 50
#define KICKER_HALL_EFFECT_THRESHOLD 450

// encoder count to release after pulling in
#define KICKER_RELEASE_COUNT 7000

extern Encoder winchEncoder;

class KickerClass {
  public:
    KickerClass();

    void begin(void);
    void loop(void);

    void release(void);
    void reload(void);
    uint8_t is_winching(void);
  private:
    elapsedMillis solenoid_active;
    /*
      winching:
      0: not winching
      1: pulling in
      2: releasing rope
    */
    uint8_t winching;
};

extern KickerClass Kicker;

#endif