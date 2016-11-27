#include "Receiver.h"

SBUS x8r(SERIAL_PORT);
ReceiverClass::ReceiverClass(void) {

}

void ReceiverClass::begin(void) {
  x8r.begin();
}

void ReceiverClass::loop(void) {
  for(uint8_t i = 1; i <= 10; i++) {
    pChannels[i] = get_channel(i);
  }

  x8r.read(&channels[0], &failSafe, &lostFrames);
}

int16_t ReceiverClass::scale(int16_t value, uint8_t channel) {
  return max(min(map(value, min[channel], max[channel], SCALE_MIN, SCALE_MAX), SCALE_MAX), SCALE_MIN);
}

uint8_t ReceiverClass::get_edge(uint8_t channel, Edge edge) {
  if (!!pChannels[channel] != !!get_channel(channel) && !!get_channel(channel) == ((uint8_t) edge)) {
    return 1;
  }

  return 0;
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
