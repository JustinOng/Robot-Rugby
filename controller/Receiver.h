#ifndef ReceiverIncluded
#define ReceiverIncluded

#include <Arduino.h>
#include "Channel.h"

// time in us from last_rising_edge after which will be treated as stale value
#define RECEIVER_TIMEOUT 15000

// duty_cycle when joystick is not activated
#define RECEIVER_ZERO 162

// period of pwm signal in microseconds
#define PWM_PERIOD 14286

// max duty cycle of the pwm signal so it can be scaled up for higher resolution
#define MAX_DUTY_CYCLE 40

#define DUTY_CYCLE_SCALE_FACTOR 255/MAX_DUTY_CYCLE

// indexed from 1 because thats how the receiver is labelled physically
#define INPUT_CH1 19
#define INPUT_CH2 18
#define INPUT_CH3 17
#define INPUT_CH4 16
#define INPUT_CH5 15
#define INPUT_CH6 14

class ReceiverClass {
  private:
    volatile Channel channels[6];
    volatile uint32_t last_rising_edge = 0;
  public:
    ReceiverClass(void);
    void update_duty_cycle(uint8_t channel);
    void update_rising_edge(uint8_t channel);
    void begin(void);

    int8_t get_channel(uint8_t channel);
};

extern ReceiverClass Receiver;

void isr_ch1(void);
void isr_ch2(void);
void isr_ch3(void);
void isr_ch4(void);
void isr_ch5(void);
void isr_ch6(void);

#endif
