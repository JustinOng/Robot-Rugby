#ifndef kickerIncluded
#define kickerIncluded

#include "Arduino.h"
#include <Encoder.h>
#include "pin_config.h"

#include "Motors.h"

#define WINCH_SOLENOID_PULSE_LENGTH 50
#define KICKER_HALL_EFFECT_THRESHOLD 450
#define KICKER_HALL_EFFECT_HYSTERESIS 100

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
    uint8_t is_loaded(void);

    // tracks the millis since the kicker was last loaded
    elapsedMillis last_loaded;
  private:
    elapsedMillis solenoid_active;
    /*
      winching:
      0: not winching
      1: pulling in
      2: releasing rope
    */
    uint8_t winching;
    uint8_t loaded;
};

extern KickerClass Kicker;

#endif
