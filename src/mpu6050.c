#include "mpu6050.h"

esp_err_t mpu6050_write_byte(uint8_t reg_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t mpu6050_read_bytes(uint8_t reg_addr, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}
esp_err_t mpu6050_whoami(void) {
    // Check if the sensor is connected
    uint8_t whoami;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_WHO_AM_I_REG, &whoami, sizeof(whoami));
    if (ret != ESP_OK) return ret;
    if (whoami != MPU6050_WHO_AM_I) return ESP_FAIL;
    return ESP_OK;
}
esp_err_t mpu6050_wake(void) {
    // Wake up MPU6050 from sleep mode
    uint8_t power_reg;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_PWR_MGMT_1_REG, &power_reg, sizeof(power_reg));
    if (ret != ESP_OK) return ret;
    power_reg |= 1 << 6;
    ret = mpu6050_write_byte(MPU6050_PWR_MGMT_1_REG, power_reg);
    return ret;
}

esp_err_t mpu6050_sleep(void) {
    // Put MPU6050 to sleep mode
    uint8_t power_reg;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_PWR_MGMT_1_REG, &power_reg, sizeof(power_reg));
    if (ret != ESP_OK) return ret;
    power_reg &= ~ (1 << 6);
    ret = mpu6050_write_byte(MPU6050_PWR_MGMT_1_REG, power_reg);
    return ret;
}

esp_err_t mpu6050_set_gyro_fs(gyro_config_t fs){
    uint8_t data;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_GYRO_CONFIG, &data, sizeof(data));
    if (ret != ESP_OK) return ret;
    data &= ~(3 << 3);
    data |= fs << 3;
    ret = mpu6050_write_byte(MPU6050_GYRO_CONFIG, data);
    return ret;
}
esp_err_t mpu6050_set_accel_fs(accel_config_t fs){
    uint8_t data;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_ACCEL_CONFIG, &data, sizeof(data));
    if (ret != ESP_OK) return ret;
    data &= ~(3 << 3);
    data |= fs << 3;
    ret = mpu6050_write_byte(MPU6050_ACCEL_CONFIG, data);
    return ret;
}
esp_err_t mpu6050_get_gyro_fs(gyro_config_t *fs){
    uint8_t data;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_GYRO_CONFIG, &data, sizeof(data));
    if (ret != ESP_OK) return ret;
    *fs = (data >> 3) & 3;
    return ret;
}
esp_err_t mpu6050_get_accel_fs(accel_config_t *fs){
    uint8_t data;
    esp_err_t ret = mpu6050_read_bytes(MPU6050_ACCEL_CONFIG, &data, sizeof(data));
    if (ret != ESP_OK) return ret;
    *fs = (data >> 3) & 3;
    return ret;
    
}

esp_err_t read_mpu6050_accel(int16_t *accel) {
    uint8_t data[6];
    esp_err_t ret = mpu6050_read_bytes(MPU6050_ACCEL_XOUT_H, data, 6);
    if (ret != ESP_OK) return ret;

    // Accelerometer
    accel[0] = (data[0] << 8) | data[1];
    accel[1] = (data[2] << 8) | data[3];
    accel[2] = (data[4] << 8) | data[5];
    return ret;
}

esp_err_t read_mpu6050_gyro(int16_t *gyro) {
    uint8_t data[6];
    esp_err_t ret = mpu6050_read_bytes(MPU6050_GYRO_XOUT_H, data, 6);
    if (ret != ESP_OK) return ret;

    // Gyroscope
    gyro[0] = (data[0] << 8) | data[1];
    gyro[1] = (data[2] << 8) | data[3];
    gyro[2] = (data[4] << 8) | data[5];
    return ret;
}