#ifndef currentMonitorIncluded
#define currentMonitorIncluded

#define CS_PINS A10, A11, A12, A13, A14, A15
#define NUM_PINS 6

#include "Arduino.h"

class CurrentMonitorClass {
  public:
    CurrentMonitorClass(void);

    void begin(void);
    void loop(void);
  private:
    uint8_t pins[NUM_PINS] = {CS_PINS};
};

extern CurrentMonitorClass Current_Monitor;

#endif
