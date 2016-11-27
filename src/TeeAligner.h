#ifndef teeAlignerIncluded
#define teeAlignerIncluded

#include "NewPing.h"
#include "pin_config.h"

// max distance for ultrasonic sensor in cm
#define ULTRASONIC_MAX 50

#define PING_INTERVAL

NewPing Ultrasonic_Left(ULTRASONIC_1_PIN, ULTRASONIC_1_PIN, ULTRASONIC_MAX);
NewPing Ultrasonic_Right(ULTRASONIC_2_PIN, ULTRASONIC_2_PIN, ULTRASONIC_MAX);

class TeeAlignerClass {
  public:
    TeeAlignerClass(void);

    void begin(void);
    void loop(void);

    void activate(void);
  private:
    uint8_t active;
    elapsedMillis last_ping;

    float left_distance, right_distance;
};

extern TeeAlignerClass TeeAligner;

#endif
