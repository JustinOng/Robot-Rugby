#include "Receiver.h"
#include "SBUS.h"

SBUS x8r(2);
ReceiverClass::ReceiverClass(void) {

}

void ReceiverClass::begin(void) {
  x8r.begin();
}

void ReceiverClass::loop(void) {
  x8r.read(&channels[0], &failSafe, &lostFrames);
}

int16_t ReceiverClass::scale(int16_t value, uint8_t channel) {
  return max(min(map(value, min[channel], max[channel], SCALE_MIN, SCALE_MAX), SCALE_MAX), SCALE_MIN);
}

int16_t ReceiverClass::get_channel(uint8_t channel) {
  int16_t scaled;

  switch(channel) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (failSafe) {
        return 0;
      }

      scaled = scale(channels[channel-1], channel-1);

      if (invert[channel-1]) {
        return scaled * -1;
      }
      else {
        return scaled;
      }

      break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      if (channels[channel-1] > 1100) return 2;
      if (channels[channel-1] > 200) return 1;
      return 0;
      break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
      return channels[channel-1];
      break;
    default:
      return 0;
  }
}

ReceiverClass Receiver;
