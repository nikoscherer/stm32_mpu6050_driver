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
struct MPU6050Data {
  int16_t accelerometer_data[3] = {};
  int16_t temperature_data = 0;
  int16_t gyroscope_data[3] = {};
};

class MPU6050 {
 public:
  MPU6050(I2C_HandleTypeDef* i2c_handle, uint8_t device_address);

  MPU6050Status read(MPU6050Data& data);

  MPU6050Status setPowerMode(
      MPU6050PowerMode power_mode,
      MPU6050TemperatureSensorMode* temperature_sensor_mode);

  static inline float accelToGs(int16_t raw_value) {
    return static_cast<float>(raw_value) / kAccelScale2G;
  }

  static inline float gyroToDPS(int16_t raw_value) {
    return static_cast<float>(raw_value) / kGyroScale250DPS;
  }

 private:
  MPU6050Status getStatus(HAL_StatusTypeDef status);

  I2C_HandleTypeDef* i2c_handle_;
  uint8_t device_address_;

  static constexpr uint8_t kRegisterPowerManagement1_ = 0x6B;
  static constexpr uint8_t kRegisterAccelXOutH_ = 0x3B;
  static constexpr uint8_t kDataLength_ =
      14;  // Length of registers for sensor measurements (59-64 Acceleration
           // Measurements, 65-66 Temperature Measurements, 67-72 Gyroscope
           // Measurements)

  static constexpr float kAccelScale2G = 16384.0f;
  static constexpr float kGyroScale250DPS = 131.0f;

  static constexpr uint8_t kTimeoutMs = 25;
};