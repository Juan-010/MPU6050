#include "include.h"

#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "driver/i2c.h"
#include "mpu6050.h"
#include "esp_system.h"
#include "esp_log.h"

static const char *TAG = "sampleTask";

#define I2C_MASTER_SCL_IO 22      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 21      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

static mpu6050_handle_t mpu6050 = NULL;

TaskHandle_t xSampleTaskHandle = NULL;

mpu6050_acce_value_t cal_acce;
mpu6050_gyro_value_t cal_gyro;

void compensate (mpu6050_acce_value_t *acce, mpu6050_gyro_value_t *gyro){
    acce->acce_x -= cal_acce.acce_x;
    acce->acce_y -= cal_acce.acce_y;
    acce->acce_z -= cal_acce.acce_z;
    gyro->gyro_x -= cal_gyro.gyro_x;
    gyro->gyro_y -= cal_gyro.gyro_y;
    gyro->gyro_z -= cal_gyro.gyro_z;
}

void calibrate (int count){
    mpu6050_acce_value_t sum_acce;
    mpu6050_gyro_value_t sum_gyro;
    memset(&sum_acce, 0, sizeof(mpu6050_acce_value_t));
    memset(&sum_gyro, 0, sizeof(mpu6050_gyro_value_t));
    mpu6050_wake_up(mpu6050);
    for (size_t i = 0; i < count; i++)
    {
        mpu6050_get_acce(mpu6050, &cal_acce);
        mpu6050_get_gyro(mpu6050, &cal_gyro);
        sum_acce.acce_x += cal_acce.acce_x;
        sum_acce.acce_y += cal_acce.acce_y;
        sum_acce.acce_z += cal_acce.acce_z;
        sum_gyro.gyro_x += cal_gyro.gyro_x;
        sum_gyro.gyro_y += cal_gyro.gyro_y;
        sum_gyro.gyro_z += cal_gyro.gyro_z;
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    cal_acce.acce_x = sum_acce.acce_x / count;
    cal_acce.acce_y = sum_acce.acce_y / count;
    cal_acce.acce_z = sum_acce.acce_z / count;
    cal_gyro.gyro_x = sum_gyro.gyro_x / count;
    cal_gyro.gyro_y = sum_gyro.gyro_y / count;
    cal_gyro.gyro_z = sum_gyro.gyro_z / count;
    mpu6050_sleep(mpu6050);
    ESP_LOGI(TAG, "Calibration values: acce_x=%.2f, acce_y=%.2f, acce_z=%.2f, gyro_x=%.2f, gyro_y=%.2f, gyro_z=%.2f", 
            cal_acce.acce_x, cal_acce.acce_y, cal_acce.acce_z, cal_gyro.gyro_x, cal_gyro.gyro_y, cal_gyro.gyro_z);
}

void vSampleTask(void *pvParameters) {

    uint8_t mpu6050_deviceid;

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
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C config returned error");   
    }
    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "I2C install returned error");   
    }
    
    mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
    if (mpu6050 == nullptr)
    {
        ESP_LOGE(TAG, "MPU6050 create returned NULL");   
    }

    ret = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_config returned error");   
    }

    ESP_LOGI(TAG, "MPU self-test start.");

    ret = mpu6050_wake_up(mpu6050);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_wake_up returned error");   
    }

    ret = mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_get_deviceid returned error");   
    }
    if (mpu6050_deviceid != MPU6050_WHO_AM_I_VAL)
    {
        ESP_LOGE(TAG, "Who Am I register does not contain expected data");   
    }
    ret = mpu6050_get_acce(mpu6050, &data.acce);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_get_acce returned error");   
    }
    ESP_LOGI(TAG, "acce_x:%.2f, acce_y:%.2f, acce_z:%.2f", data.acce.acce_x, data.acce.acce_y, data.acce.acce_z);

    ret = mpu6050_get_gyro(mpu6050, &data.gyro);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_get_gyro returned error");   
    }
    ESP_LOGI(TAG, "gyro_x:%.2f, gyro_y:%.2f, gyro_z:%.2f", data.gyro.gyro_x, data.gyro.gyro_y, data.gyro.gyro_z);

    ret = mpu6050_get_temp(mpu6050, &data.temp);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_get_temp returned error");   
    }
    ESP_LOGI(TAG, "t:%.2f", data.temp.temp);

    ret = mpu6050_sleep(mpu6050);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "mpu6050_sleep returned error");   
    }
    
    ESP_LOGI(TAG, "MPU self-test end.");

    ESP_LOGI(TAG, "Calibrating MPU6050...");
    calibrate(50);
    ESP_LOGI(TAG, "Calibration done.");

    ESP_LOGI(TAG, "SampleTask started.");

    while (1) {
        mpu6050_wake_up(mpu6050);
        mpu6050_get_acce(mpu6050, &data.acce);
        mpu6050_get_gyro(mpu6050, &data.gyro);
        mpu6050_get_temp(mpu6050, &data.temp);
        mpu6050_sleep(mpu6050);
        data.ctr++;
        compensate(&data.acce, &data.gyro);

        xQueueSendToBack(xUploadQueue, (void *)&data, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}