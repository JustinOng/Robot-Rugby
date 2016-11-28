#ifndef teeAlignerIncluded
#define teeAlignerIncluded

#include "NewPing.h"
#include "pin_config.h"

// max distance for ultrasonic sensor in cm
#define ULTRASONIC_MAX 50

#define PING_INTERVAL

extern NewPing Ultrasonic_Left;
extern NewPing Ultrasonic_Right;

class TeeAlignerClass {
  public:
    TeeAlignerClass(void);

    void begin(void);
    void loop(void);

    void activate(void);
    void deactivate(void);
    uint8_t is_active(void);
  private:
    uint8_t active;
    elapsedMillis last_ping;

    float left_distance, right_distance;
};

extern TeeAlignerClass TeeAligner;

#endif
