#include "ReceiverInput.h"

ReceiverInputClass::ReceiverInputClass(void) {
  
}

void ReceiverInputClass::begin(void) {
  // create 6 channel structs and store in array
  /*for(uint8_t i = 0; i < 6; i++) {
    channels.index = i;
    //channels[i] = &channel;
  }*/
  
  attachInterrupt(INPUT_CH1, isr_ch1, CHANGE);
  attachInterrupt(INPUT_CH2, isr_ch2, CHANGE);
  attachInterrupt(INPUT_CH3, isr_ch3, CHANGE);
  attachInterrupt(INPUT_CH4, isr_ch4, CHANGE);
  attachInterrupt(INPUT_CH5, isr_ch5, CHANGE);
  attachInterrupt(INPUT_CH6, isr_ch6, CHANGE);
}

void ReceiverInputClass::update_rising_edge(uint8_t channel) {
  channels[channel-1].last_rising_edge = micros();
}

void ReceiverInputClass::update_duty_cycle(uint8_t channel) {
  channels[channel-1].duty_cycle = (micros() - channels[channel-1].) * 255 / PWM_PERIOD;
}

void isr_ch1(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH1) == 1) {
    ReceiverInput.update_rising_edge(1);
  }
  else {
    ReceiverInput.update_duty_cycle(1);
  }
}

void isr_ch2(void) {
  ReceiverInput.update_duty_cycle(2);
}

void isr_ch3(void) {
  ReceiverInput.update_duty_cycle(3);
}

void isr_ch4(void) {
  ReceiverInput.update_duty_cycle(4);
}

void isr_ch5(void) {
  ReceiverInput.update_duty_cycle(5);
}

void isr_ch6(void) {
  ReceiverInput.update_duty_cycle(6);
}

ReceiverInputClass ReceiverInput;
