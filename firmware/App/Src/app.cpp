#include "app.hpp"

#include <cstdio>
#include <debug.hpp>

#include "mpu6050.hpp"

extern I2C_HandleTypeDef hi2c1;
static MPU6050 mpu_sensor = MPU6050(&hi2c1, 0x68 << 1);

int app_main(void) {
  if (mpu_sensor.setPowerMode(MPU6050PowerMode::kAwake, nullptr) !=
      MPU6050Status::kOk) {
    printf("Failed to initialize MPU6050!");
  }
  HAL_Delay(100);

  return 0;
}

int app_loop(void) {
  MPU6050Data sensor_data;
  if (mpu_sensor.read(sensor_data) == MPU6050Status::kOk) {
    float a[3] = {MPU6050::accelToGs(sensor_data.accelerometer_data[0]),
                  MPU6050::accelToGs(sensor_data.accelerometer_data[1]),
                  MPU6050::accelToGs(sensor_data.accelerometer_data[2])};
    float g[3] = {MPU6050::gyroToDPS(sensor_data.gyroscope_data[0]),
                  MPU6050::gyroToDPS(sensor_data.gyroscope_data[1]),
                  MPU6050::gyroToDPS(sensor_data.gyroscope_data[2])};

    printf(
        "Accel X: %f | Accel Y: %f | Accel Z: %f || Gyro X: %f | Gyro Y: %f | "
        "Gyro Z: %f \r\n",
        a[0], a[1], a[2], g[0], g[1], g[2]);
  } else {
    printf("Failed to read MPU6050!\r\n");
  }

  // Add a small delay so you don't flood the terminal (consider non-blocking
  // millis() later)
  HAL_Delay(5);

  return 0;
}