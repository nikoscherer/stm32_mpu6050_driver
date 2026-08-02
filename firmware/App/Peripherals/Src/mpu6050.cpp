#include "mpu6050.hpp"

MPU6050::MPU6050(I2C_HandleTypeDef* i2c_handle, uint8_t device_address)
    : i2c_handle_(i2c_handle), device_address_(device_address) {}

MPU6050Status MPU6050::read(MPU6050Data& data) {
  uint8_t rx_buffer[kDataLength_] =
      {};  // initialize a buffer to store registry data for accelerometer,
           // temperature, and gyroscope

  // Reads from Registry 59-72 (sensor measurements) onto rx_buffer
  const HAL_StatusTypeDef rx_status = HAL_I2C_Mem_Read(
      i2c_handle_, device_address_, kRegisterAccelXOutH_, I2C_MEMADD_SIZE_8BIT,
      rx_buffer, sizeof(rx_buffer), kTimeoutMs);

  // Check status of memory read
  if (const MPU6050Status status = getStatus(rx_status);
      status != MPU6050Status::kOk) {
    return status;
  }

  // Writes raw data to MPU6050Data struct
  data.accelerometer_data[0] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[0] << 8) |
                 rx_buffer[1]));  // ACCEL_XOUT_H, ACCEL_XOUT_L
  data.accelerometer_data[1] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[2] << 8) |
                 rx_buffer[3]));  // ACCEL_YOUT_H, ACCEL_YOUT_L
  data.accelerometer_data[2] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[4] << 8) |
                 rx_buffer[5]));  // ACCEL_ZOUT_H, ACCEL_ZOUT_L

  data.temperature_data =
      static_cast<int16_t>((uint16_t)((rx_buffer[6] << 8) | rx_buffer[7])) /
          340 +
      36.53;  // TEMP_OUT_H, TEMP_OUT_L

  data.gyroscope_data[0] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[8] << 8) |
                 rx_buffer[9]));  // GYRO_XOUT_H, GYRO_XOUT_L
  data.gyroscope_data[1] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[10] << 8) |
                 rx_buffer[11]));  // GYRO_YOUT_H, GYRO_YOUT_L
  data.gyroscope_data[2] = static_cast<int16_t>(
      (uint16_t)((rx_buffer[12] << 8) |
                 rx_buffer[13]));  // GYRO_ZOUT_H, GYRO_ZOUT_L

  return MPU6050Status::kOk;
}

MPU6050Status MPU6050::setPowerMode(
    MPU6050PowerMode power_mode,
    MPU6050TemperatureSensorMode* temperature_sensor_mode) {
  uint8_t rx_buffer = 0;

  // Reads from Registry 107 Power Management 1 onto rx_buffer
  const HAL_StatusTypeDef rx_status = HAL_I2C_Mem_Read(
      i2c_handle_, device_address_, kRegisterPowerManagement1_,
      I2C_MEMADD_SIZE_8BIT, &rx_buffer, sizeof(rx_buffer), kTimeoutMs);

  // Check status of memory read
  if (const MPU6050Status status = getStatus(rx_status);
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
  const HAL_StatusTypeDef tx_status = HAL_I2C_Mem_Write(
      i2c_handle_, device_address_, kRegisterPowerManagement1_,
      I2C_MEMADD_SIZE_8BIT, &tx_buffer, sizeof(tx_buffer), kTimeoutMs);

  // Returns final status after writing data to registry
  return getStatus(tx_status);
}

// Wrapper for converting HAL_StatusTypeDef to MPU6050Status
MPU6050Status MPU6050::getStatus(const HAL_StatusTypeDef status) {
  if (status != HAL_StatusTypeDef::HAL_OK) {
    return status == HAL_StatusTypeDef::HAL_TIMEOUT ? MPU6050Status::kTimeout
                                                    : MPU6050Status::kError;
  }

  return MPU6050Status::kOk;
}