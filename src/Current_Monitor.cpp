#include "Current_Monitor.h"

CurrentMonitorClass::CurrentMonitorClass(void) {

}

void CurrentMonitorClass::begin(void) {

}

void CurrentMonitorClass::loop(void) {
  for(uint8_t i = 0; i < NUM_PINS; i++) {
    uint16_t adc_value = analogRead(pins[i]);

    float cs_voltage = (float) adc_value / 1024 * 3.3;

    float current = cs_voltage / 0.13;

    /*Serial1.print("M");
    Serial1.print(i);
    Serial1.print(": ");
    Serial1.print(current);
    Serial1.print("A ");*/
  }
  //Serial1.print("\r\n");
}

CurrentMonitorClass Current_Monitor;
