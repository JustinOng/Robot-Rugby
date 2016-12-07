#ifndef gyroHeaderIncluded
#define gyroHeaderIncluded

#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

class GyroClass {
  public:
    GyroClass(void);

    void begin(void);
    void loop(void);

  private:
    MPU6050 mpu;
    uint8_t init_success;
    uint8_t packet_size;
    uint8_t fifo_buffer[64];
    Quaternion q;
    float euler[3];
};

extern GyroClass Gyro;

#endif
