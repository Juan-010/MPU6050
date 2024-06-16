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

esp_err_t BMP280::read()
{
    esp_err_t ret = readValues();
    if (ret != ESP_OK)
    {
        return ret;
    }
    
    int32_t adc_P = (__read_buffer[1] << 12) | (__read_buffer[2] << 4) | (__read_buffer[3] >> 4);
    int32_t adc_T = (__read_buffer[4] << 12) | (__read_buffer[5] << 4) | (__read_buffer[6] >> 4);

    _temp = compensateTemp(adc_T);
    _press = compensatePress(adc_P);
    
    return ESP_OK;
}
