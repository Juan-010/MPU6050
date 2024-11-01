#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mpu6050.h"
#include "uart_config.h"
#include "esp_log.h"
#include "sdkconfig.h"

void app_main(void) {
    // Checks for fatal errors
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_ERROR_CHECK(uart_init());
    ESP_ERROR_CHECK(mpu6050_whoami());
    esp_err_t ret;

    // We refrain from using ESP_ERROR_CHECK here because error is not fatal
    ret = mpu6050_set_accel_fs(ACCEL_FS_SEL_2G);
    if (ret != ESP_OK) 
        ESP_LOGE("MPU6050", "Error setting accelerometer full scale: %d, will use default value of +-2G", ret);
    
    ret = mpu6050_set_gyro_fs(GYRO_FS_SEL_250);
    if (ret != ESP_OK) 
        ESP_LOGE("MPU6050", "Error setting gyroscope full scale: %d, will use default value of +-250°/s", ret);
    
    int16_t accel[3], gyro[3];
    uint8_t counter = 0;
    mpu6050_data_t data;
    
    while (1) {
        ret = mpu6050_wake();
        if (ret != ESP_OK) 
            ESP_LOGE("MPU6050", "Error waking up MPU6050: %d", ret);

        ret = read_mpu6050_accel(accel);
        if (ret != ESP_OK) 
            ESP_LOGE("MPU6050", "Error reading accelerometer: %d", ret);

        ret = read_mpu6050_gyro(gyro);
        if (ret != ESP_OK) 
            ESP_LOGE("MPU6050", "Error reading gyroscope: %d", ret);

        ret = mpu6050_sleep();
        if (ret != ESP_OK) 
            ESP_LOGE("MPU6050", "Error putting MPU6050 to sleep: %d", ret);

        data = (mpu6050_data_t) {
            .start_byte = 0x1B,
            .counter = counter++,
            .accel_x = accel[0],
            .accel_y = accel[1],
            .accel_z = accel[2],
            .gyro_x = gyro[0],
            .gyro_y = gyro[1],
            .gyro_z = gyro[2],
        };
        uart_write_bytes(UART_PORT_NUM, &data, sizeof(data));
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
