/**
 * @file bmp280.cpp
 * @brief BMP280 driver implementation file.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#include "../include/bmp280.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"

static const char *TAG = "BMP280";

uint8_t bmp280_mode_values[6][2] = {
                                {BMP280_CTRL_MEAS_LOW_POWER, BMP280_REG_CONFIG_LOW_POWER},
                                {BMP280_CTRL_MEAS_DYNAMIC, BMP280_REG_CONFIG_DYNAMIC}, 
                                {BMP280_CTRL_MEAS_WEATHER, BMP280_REG_CONFIG_WEATHER},
                                {BMP280_CTRL_MEAS_ELEVATOR, BMP280_REG_CONFIG_ELEVATOR},
                                {BMP280_CTRL_MEAS_DROP, BMP280_REG_CONFIG_DROP},
                                {BMP280_CTRL_MEAS_NAVIGATION, BMP280_REG_CONFIG_NAVIGATION}
                                };

esp_err_t BMP280::init(Mode mode)
{
    _mode = mode;
    uint8_t id = readReg(BMP280_REG_ID);
    ESP_LOGI(TAG, "BMP280 ID: 0x%x", id);

    if (id != 0x58)
    {
        ESP_LOGE(TAG, "No se pudo encontrar un sensor BMP280 válido, verificar la conexión!");
        return ESP_FAIL;
    }

    // Leer datos de calibración
    readCalibrationData();

    // Resetear el sensor
    esp_err_t ret = writeReg(BMP280_REG_RESET, 0xB6);
    if (ret != ESP_OK)
    {
        return ret;
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    return setMode(mode);
}

esp_err_t BMP280::setMode(Mode mode)
{   
    _mode = mode;
    return writeReg(BMP280_REG_CTRL_MEAS, bmp280_mode_values[(uint8_t) _mode], 2);
}

esp_err_t BMP280::read()
{
    uint8_t read_buffer[6];
    esp_err_t ret = readReg(BMP280_REG_PRESS_MSB, read_buffer, 6);
    if (ret != ESP_OK)
    {
        return ret;
    }
    
    int32_t adc_P = (read_buffer[0] << 12) | (read_buffer[1] << 4) | (read_buffer[2] >> 4);
    int32_t adc_T = (read_buffer[3] << 12) | (read_buffer[4] << 4) | (read_buffer[5] >> 4);

    _press = compensatePress(adc_P);
    _temp = compensateTemp(adc_T);
    
    return ESP_OK;
}
