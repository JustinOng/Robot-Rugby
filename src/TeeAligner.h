#ifndef teeAlignerIncluded
#define teeAlignerIncluded

#include "NewPing.h"
#include "pin_config.h"
#include "Motors.h"

// max distance for ultrasonic sensor in cm
#define ULTRASONIC_MAX 100
#define ULTRASONIC_SAMPLES 8

#define PING_INTERVAL 1
#define MOTOR_INTERVAL 50

#define LEFT_THRESHOLD 12.20
#define RIGHT_THRESHOLD 13.45

#define KP_L 6
#define KP_R 6

#define KI_L 0.4
#define KI_R 0.4

#define MAX_L 20
#define MAX_R 20

#define DECAY_RATE 0.95;

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
    elapsedMillis last_motor_set;
    float left_distance_running_average;
    float right_distance_running_average;
    float left_integral;
    float right_integral;

    void init(void);
};

extern TeeAlignerClass TeeAligner;

#endif
