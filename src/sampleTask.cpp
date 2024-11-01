#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_system.h"
#include "include.h"


#include "mpu6050.h"

static const char *TAG = "sampleTask";

#define I2C_MASTER_SCL_IO 22      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

TaskHandle_t xSampleTaskHandle = NULL;

void vSampleTask(void *pvParameters) {

    Trama_t data;
    data.id = TRAMA_ID;
    data.ctr = 0;

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C config returned error");
    }
    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C install returned error");
    }

    ESP_LOGI(TAG, "I2C driver installed.");

    ret = mpu6050_init(I2C_MASTER_NUM);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error initializing MPU6050: %d", ret);
    }

    ret = mpu6050_whoami();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error checking MPU6050 identity: %d", ret);
    }

    ret = mpu6050_wake();
    if (ret != ESP_OK)
        ESP_LOGE(TAG, "Error waking up MPU6050: %d", ret);

    ret = read_mpu6050_accel((int16_t*) data.acce);
    if (ret != ESP_OK)
        ESP_LOGE(TAG, "Error reading accelerometer: %d", ret);

    ret = read_mpu6050_gyro((int16_t*) data.gyro);
    if (ret != ESP_OK)
        ESP_LOGE(TAG, "Error reading gyroscope: %d", ret);

    ret = mpu6050_sleep();
    if (ret != ESP_OK)
        ESP_LOGE(TAG, "Error putting MPU6050 to sleep: %d", ret);

    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(100);
    BaseType_t xWasDelayed;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount ();

    ESP_LOGI(TAG, "MPU sampling started.");

    while (1) {
        xWasDelayed = xTaskDelayUntil( &xLastWakeTime, xFrequency );
        ret = mpu6050_wake();
        ret = read_mpu6050_accel((int16_t*) data.acce);
        ret = read_mpu6050_gyro((int16_t*) data.gyro);
        ret = mpu6050_sleep();
        data.ctr++;
        xQueueSendToBack(xUploadQueue, (void *)&data, portMAX_DELAY);
    }
}