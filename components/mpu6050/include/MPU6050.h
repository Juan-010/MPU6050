#ifndef MPU6050_H
#define MPU6050_H

#include "esp_err.h"
#include "driver/i2c.h"

/** @file mpu6050.h
 *  @brief MPU6050 sensor driver for ESP32 using I2C.
 *
 *  This file contains function declarations and macros to interface with the MPU6050 sensor, 
 *  enabling accelerometer and gyroscope data collection, as well as sensor configuration.
 */

/** MPU6050 sensor I2C address */
#define MPU6050_SENSOR_ADDR 0x68

/** MPU6050 Who Am I register address */
#define MPU6050_WHO_AM_I_REG 0x75
/** MPU6050 Who Am I expected value */
#define MPU6050_WHO_AM_I 0x68

/** MPU6050 Power Management register address */
#define MPU6050_PWR_MGMT_1_REG 0x6B
/** MPU6050 Sleep mode bit */
#define MPU6050_SLEEP_BIT 0x40

/** Accelerometer X-axis high byte register */
#define MPU6050_ACCEL_XOUT_H 0x3B
/** Accelerometer configuration register address */
#define MPU6050_ACCEL_CONFIG 0x1C

/** Gyroscope X-axis high byte register */
#define MPU6050_GYRO_XOUT_H 0x43
/** Gyroscope configuration register address */
#define MPU6050_GYRO_CONFIG 0x1B

/** @brief Gyroscope configuration options */
typedef enum {
    GYRO_FS_SEL_250 = 0,   /**< Gyroscope full scale ±250°/s */
    GYRO_FS_SEL_500 = 1,   /**< Gyroscope full scale ±500°/s */
    GYRO_FS_SEL_1000 = 2,  /**< Gyroscope full scale ±1000°/s */
    GYRO_FS_SEL_2000 = 3   /**< Gyroscope full scale ±2000°/s */
} gyro_config_t;

/** @brief Accelerometer configuration options */
typedef enum {
    ACCEL_FS_SEL_2G = 0,   /**< Accelerometer full scale ±2g */
    ACCEL_FS_SEL_4G = 1,   /**< Accelerometer full scale ±4g */
    ACCEL_FS_SEL_8G = 2,   /**< Accelerometer full scale ±8g */
    ACCEL_FS_SEL_16G = 3   /**< Accelerometer full scale ±16g */
} accel_config_t;


/**
 * @brief Initialize the MPU6050 sensor
 * 
 * @param port  I2C port number
 * @return esp_err_t  ESP_OK if successful, ESP_FAIL otherwise
 */
esp_err_t mpu6050_init(i2c_port_t port);

/** @brief Write a byte to a MPU6050 register
 *
 *  @param reg_addr Register address to write to
 *  @param data Byte data to write
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_write_byte(uint8_t reg_addr, uint8_t data);

/** @brief Read multiple bytes from MPU6050
 *
 *  @param reg_addr Register address to start reading from
 *  @param data Pointer to buffer where data will be stored
 *  @param len Number of bytes to read
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_read_bytes(uint8_t reg_addr, uint8_t *data, size_t len);

/** @brief Check MPU6050 identity
 *
 *  @return ESP_OK if device ID matches, error code otherwise
 */
esp_err_t mpu6050_whoami(void);

/** @brief Wake up MPU6050 from sleep mode
 *
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_wake(void);

/** @brief Put MPU6050 into sleep mode
 *
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_sleep(void);

/** @brief Set gyroscope full-scale range
 *
 *  @param fs Gyroscope full scale setting
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_set_gyro_fs(gyro_config_t fs);

/** @brief Set accelerometer full-scale range
 *
 *  @param fs Accelerometer full scale setting
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_set_accel_fs(accel_config_t fs);

/** @brief Get current gyroscope full-scale range
 *
 *  @param fs Pointer to store current gyroscope full scale setting
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_get_gyro_fs(gyro_config_t *fs);

/** @brief Get current accelerometer full-scale range
 *
 *  @param fs Pointer to store current accelerometer full scale setting
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t mpu6050_get_accel_fs(accel_config_t *fs);

/** @brief Read accelerometer data
 *
 *  @param accel Pointer to store accelerometer data (X, Y, Z axes)
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t read_mpu6050_accel(int16_t *accel);

/** @brief Read gyroscope data
 *
 *  @param gyro Pointer to store gyroscope data (X, Y, Z axes)
 *  @return ESP_OK on success, error code otherwise
 */
esp_err_t read_mpu6050_gyro(int16_t *gyro);

#endif
