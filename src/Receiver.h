#ifndef ReceiverIncluded
#define ReceiverIncluded

#include <Arduino.h>

#define SCALE_MIN -100
#define SCALE_MAX 100

class ReceiverClass {
  private:
    volatile uint32_t last_rising_edge = 0;

    int16_t min[4] = {
      368,
      368,
      368,
      368
    };

    int16_t max[4] = {
      1680,
      1680,
      1680,
      1680
    };

    int16_t ctr[4] = {
      1014,
      1024,
      1024,
      1028
    };

    uint8_t invert[4] = {
      1,
      0,
      0  ,
      1
    };

  public:
    ReceiverClass(void);
    void begin(void);
    void loop(void);

    enum Edge {
      FALLING_EDGE,
      RISING_EDGE
    };

    int16_t get_channel(uint8_t channel);
    uint8_t failSafe = 1;
    uint16_t channels[16];
    uint16_t pChannels[16];

    uint8_t get_edge(uint8_t channel, Edge edge);

  private:
    uint16_t lostFrames = 0;

    int16_t scale(int16_t value, uint8_t channel);
};

extern ReceiverClass Receiver;

#endif
