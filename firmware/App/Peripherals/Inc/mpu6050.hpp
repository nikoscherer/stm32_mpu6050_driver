#pragma once

#include "main.h"

enum class MPU6050Status : uint8_t { kOk = 0, kTimeout = 1, kError = 2 };
enum class MPU6050PowerMode : uint8_t {
  kSleep = 0,
  kCycle = 1,
  kAwake = 2,
  kReset = 3
};
enum class MPU6050TemperatureSensorMode : uint8_t {
  kEnabled = 0,
  kDisabled = 1
};

class MPU6050 {
 public:
  MPU6050(I2C_HandleTypeDef* i2c_handle, uint8_t device_address);

  MPU6050Status setPowerMode(
      MPU6050PowerMode power_mode,
      MPU6050TemperatureSensorMode* temperature_sensor_mode);

 private:
  MPU6050Status getStatus(HAL_StatusTypeDef status);

  I2C_HandleTypeDef* i2c_handle_;
  uint8_t device_address_;

  static constexpr uint8_t kRegisterPowerManagement1_ = 0x6B;

  static constexpr uint8_t kTimeoutMs = 25;
};