#include "mpu6050.hpp"

MPU6050::MPU6050(I2C_HandleTypeDef* i2c_handle, uint8_t device_address)
    : i2c_handle_(i2c_handle), device_address_(device_address) {}

MPU6050Status MPU6050::setPowerMode(
    MPU6050PowerMode power_mode,
    MPU6050TemperatureSensorMode* temperature_sensor_mode) {
  uint8_t rx_buffer = 0;

  // Reads from Registry 107 Power Management 1 onto rx_buffer
  HAL_StatusTypeDef const rx_status = HAL_I2C_Mem_Read(
      i2c_handle_, device_address_, kRegisterPowerManagement1_,
      I2C_MEMADD_SIZE_8BIT, &rx_buffer, sizeof(rx_buffer), kTimeoutMs);

  // Check status of memory read
  if (MPU6050Status status = getStatus(rx_status);
      status != MPU6050Status::kOk) {
    return status;
  }

  // copy rx_buffer data to tx_buffer
  uint8_t tx_buffer{rx_buffer};

  // Change bits in tx_buffer to match desired power mode
  if (power_mode == MPU6050PowerMode::kAwake) {
    tx_buffer &= ~0x60;  // 0110 0000 binary (disables sleep & cycle)
  } else if (power_mode == MPU6050PowerMode::kSleep) {
    tx_buffer |= 0x40;   // 0100 0000 binary (enables sleep)
    tx_buffer &= ~0x20;  // 0010 0000 binary (disables cycle)
  } else if (power_mode == MPU6050PowerMode::kCycle) {
    tx_buffer |= 0x20;   // 0010 0000 binary (enables cycle)
    tx_buffer &= ~0x40;  // 0100 0000 binary (disables sleep)
  } else if (power_mode == MPU6050PowerMode::kReset) {
    tx_buffer |= 0x80;  // 1000 0000 binary (enables device reset)
  }

  // Change bits in rx_buffer to match desired temperature sensor mode (enabled
  // or disabled)
  if (temperature_sensor_mode != nullptr) {
    if (*temperature_sensor_mode == MPU6050TemperatureSensorMode::kEnabled) {
      tx_buffer &= ~0x08;  // 0000 1000 binary (enables temperature sensor)
    } else {
      tx_buffer |= 0x08;  // 0000 1000 binary (disables temperature sensor)
    }
  }

  // Writes to Registry 107 Power Management with tx_buffer
  HAL_StatusTypeDef const tx_status = HAL_I2C_Mem_Write(
      i2c_handle_, device_address_, kRegisterPowerManagement1_,
      I2C_MEMADD_SIZE_8BIT, &tx_buffer, sizeof(tx_buffer), kTimeoutMs);

  // Returns final status after writing data to registry
  return getStatus(tx_status);
}

// Wrapper for converting HAL_StatusTypeDef to MPU6050Status
MPU6050Status MPU6050::getStatus(HAL_StatusTypeDef status) {
  if (status != HAL_StatusTypeDef::HAL_OK) {
    return status == HAL_StatusTypeDef::HAL_TIMEOUT ? MPU6050Status::kTimeout
                                                    : MPU6050Status::kError;
  }

  return MPU6050Status::kOk;
}