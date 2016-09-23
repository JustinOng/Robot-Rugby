#include "Receiver.h"

ReceiverClass::ReceiverClass(void) {
  
}

void ReceiverClass::begin(void) {  
  for(uint8_t i = 14; i <= 19; i++) {
    pinMode(i, INPUT);
  }
  
  attachInterrupt(INPUT_CH1, isr_ch1, CHANGE);
  attachInterrupt(INPUT_CH2, isr_ch2, CHANGE);
  attachInterrupt(INPUT_CH3, isr_ch3, CHANGE);
  attachInterrupt(INPUT_CH4, isr_ch4, CHANGE);
  attachInterrupt(INPUT_CH5, isr_ch5, CHANGE);
  attachInterrupt(INPUT_CH6, isr_ch6, CHANGE);
}

void ReceiverClass::update_rising_edge(uint8_t channel) {
  channels[channel-1].last_rising_edge = micros();
}

void ReceiverClass::update_duty_cycle(uint8_t channel) {
  channels[channel-1].duty_cycle = (micros() - channels[channel-1].last_rising_edge) * DUTY_CYCLE_SCALE_FACTOR * 255 / PWM_PERIOD;
}

int16_t ReceiverClass::scale(int16_t value, uint8_t channel) {
  return max(min(map(value, min[channel-1], max[channel-1], -127, 127), 127), -127);
}

int16_t ReceiverClass::get_channel(uint8_t channel) {
  // if has timeout, return 0 so that the robot does not go "run away"
  if ((micros() - channels[channel-1].last_rising_edge) > RECEIVER_TIMEOUT) {
    return 0;
  }

  switch(channel) {
    case 1:
    case 6:
      return scale((int16_t) channels[0].duty_cycle - RECEIVER_ZERO, 1);
      break;
    case 2:
      return scale((((int16_t) channels[1].duty_cycle - RECEIVER_ZERO) + (channels[3].duty_cycle - RECEIVER_ZERO))/2, 2);
      break;
    case 3:
      return scale((int16_t) channels[2].duty_cycle - RECEIVER_ZERO, 3);
      break;
    case 4:
      return scale(((int16_t) channels[3].duty_cycle - RECEIVER_ZERO) - (channels[1].duty_cycle - RECEIVER_ZERO), 4);
      break;
    case 5:
      // duty cycle of ch5 is either 110 or 235, so just check if more/less than average (which is 172.5)
      return channels[4].duty_cycle > 173;
      break;
    default:
      return 0;
      break;
  }
}

void isr_ch1(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH1) == 1) {
    Receiver.update_rising_edge(1);
  }
  else {
    Receiver.update_duty_cycle(1);
  }
}

void isr_ch2(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH2) == 1) {
    Receiver.update_rising_edge(2);
  }
  else {
    Receiver.update_duty_cycle(2);
  }
}

void isr_ch3(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH3) == 1) {
    Receiver.update_rising_edge(3);
  }
  else {
    Receiver.update_duty_cycle(3);
  }
}

void isr_ch4(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH4) == 1) {
    Receiver.update_rising_edge(4);
  }
  else {
    Receiver.update_duty_cycle(4);
  }
}

void isr_ch5(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH5) == 1) {
    Receiver.update_rising_edge(5);
  }
  else {
    Receiver.update_duty_cycle(5);
  }
}

void isr_ch6(void) {
  // if rising edge, update last_rising_edge
  if (digitalReadFast(INPUT_CH6) == 1) {
    Receiver.update_rising_edge(6);
  }
  else {
    Receiver.update_duty_cycle(6);
  }
}

ReceiverClass Receiver;
