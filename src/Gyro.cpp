#include "Gyro.h"

GyroClass::GyroClass(void) {

}

void GyroClass::begin(void) {
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial1.println("Failed to detect MPU6050!");
    init_success = false;
    return;
  }

  uint8_t devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    init_success = true;
    packet_size = mpu.dmpGetFIFOPacketSize();
  }
  else {
    Serial1.print("Failed to enable DMP on the MPU6050! Error code:");
    Serial1.println(devStatus);
  }
}

void GyroClass::loop(void) {
  // if DMP ready
  if (mpu.getIntStatus() & 0x02) {
    if (mpu.getFIFOCount() < packet_size) {
      // not enough data in DMP FIFO, exit this loop and wait for next one
      return;
    }

    mpu.getFIFOBytes(fifo_buffer, packet_size);
    mpu.dmpGetQuaternion(&q, fifo_buffer);
    mpu.dmpGetEuler(euler, &q);
  }
}

GyroClass Gyro;
