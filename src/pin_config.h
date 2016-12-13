#ifndef pin_configIncluded
#define pin_configIncluded

#define WINCH_SOLENOID_PIN 11

#define WINCH_ENCODER_A 2
#define WINCH_ENCODER_B 3

#define LIFT_ENCODER_A 4
#define LIFT_ENCODER_B 5

#define GRIPPER_SERVO_PIN 12

// ultrasonic is not used. pin 1 reused to control servo relay
#define ULTRASONIC_1_PIN 32
#define ULTRASONIC_2_PIN 33

#define SERVO_RELAY_PIN 32

#define MOTOR_ENABLE_PIN 24

#define KICKER_HALL_EFFECT_PIN A0
#define LIFT_HALL_EFFECT_PIN A1

#define STATUS_LEDS_PIN 16
#endif
