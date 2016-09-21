#ifndef ReceiverInputIncluded
#define ReceiverInputIncluded

#include <Arduino.h>
#include "Channel.h"

// period of pwm signal in microseconds
#define PWM_PERIOD 14286

// indexed from 1 because thats how the receiver is labelled physically
#define INPUT_CH1 19
#define INPUT_CH2 18
#define INPUT_CH3 17
#define INPUT_CH4 16
#define INPUT_CH5 15
#define INPUT_CH6 14

class ReceiverInputClass {
  public:
    volatile Channel channels[6];
    volatile uint32_t last_rising_edge = 0;
    
    ReceiverInputClass(void);
    void update_duty_cycle(uint8_t channel);
    void update_rising_edge(uint8_t channel);
    void begin(void);
};

extern ReceiverInputClass ReceiverInput;

void isr_ch1(void);
void isr_ch2(void);
void isr_ch3(void);
void isr_ch4(void);
void isr_ch5(void);
void isr_ch6(void);

#endif
