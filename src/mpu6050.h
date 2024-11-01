#ifndef MPU6050_H
#define MPU6050_H
#include "i2c_config.h"

// MPU6050 sensor configuration
#define MPU6050_SENSOR_ADDR 0x68   // Slave address of the MPU6050 sensor

#define MPU6050_WHO_AM_I_REG 0x75  // Register for Who Am I
#define MPU6050_WHO_AM_I 0x68      // Value for Who Am I

#define MPU6050_PWR_MGMT_1_REG 0x6B // Register for Power Management
#define MPU6050_SLEEP_BIT 0x40      // Bit for sleep mode

#define MPU6050_ACCEL_XOUT_H 0x3B  // Accelerometer X-axis high byte
#define MPU6050_ACCEL_CONFIG 0x1C  // Accelerometer configuration register

#define MPU6050_GYRO_XOUT_H 0x43   // Gyroscope X-axis high byte
#define MPU6050_GYRO_CONFIG 0x1B   // Gyroscope configuration register


// MPU6050 data structure
#pragma pack(push, 1)
typedef struct {
    uint8_t start_byte;
    uint8_t counter;
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
} mpu6050_data_t;
#pragma pack(pop)

typedef enum {
    GYRO_FS_SEL_250 = 0,
    GYRO_FS_SEL_500 = 1,
    GYRO_FS_SEL_1000 = 2,
    GYRO_FS_SEL_2000 = 3
} gyro_config_t;

typedef enum {
    ACCEL_FS_SEL_2G = 0,
    ACCEL_FS_SEL_4G = 1,
    ACCEL_FS_SEL_8G = 2,
    ACCEL_FS_SEL_16G = 3
} accel_config_t;

esp_err_t mpu6050_write_byte(uint8_t reg_addr, uint8_t data);
esp_err_t mpu6050_read_bytes(uint8_t reg_addr, uint8_t *data, size_t len);

esp_err_t mpu6050_whoami(void);

esp_err_t mpu6050_wake(void);
esp_err_t mpu6050_sleep(void);

esp_err_t mpu6050_set_gyro_fs(gyro_config_t fs);
esp_err_t mpu6050_set_accel_fs(accel_config_t fs);

esp_err_t mpu6050_get_gyro_fs(gyro_config_t *fs);
esp_err_t mpu6050_get_accel_fs(accel_config_t *fs);

esp_err_t read_mpu6050_accel(int16_t *accel);
esp_err_t read_mpu6050_gyro(int16_t *gyro);

#endif