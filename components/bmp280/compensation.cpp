/**
 * @file compensation.cpp
 * @brief BMP280 compensation functions implementation file.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 * @note Functions extracted per the BMP280 datasheet from Bosch.
 */

#include "../include/bmp280.h"
#include "../include/calibration.h"
#include <stdint.h>

int32_t BMP280::compensateTemp(int32_t uncompensatedTemp)
{
    int32_t var1, var2, T;
    var1 = ((((uncompensatedTemp >> 3) - ((int32_t)_calib_data.dig_T1 << 1))) * ((int32_t)_calib_data.dig_T2)) >> 11;
    var2 = (((((uncompensatedTemp >> 4) - ((int32_t)_calib_data.dig_T1)) * ((uncompensatedTemp >> 4) - ((int32_t)_calib_data.dig_T1))) >> 12) *
            ((int32_t)_calib_data.dig_T3)) >>
           14;
    _t_fine = var1 + var2;
    T = (_t_fine * 5 + 128) >> 8;
    return T;
}

// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t BMP280::compensatePress(int32_t uncompensatedPress)
{
    int32_t var1, var2;
    uint32_t p;
    var1 = (((int32_t)_t_fine) >> 1) - (int32_t)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t)_calib_data.dig_P6);
    var2 = var2 + ((var1 * ((int32_t)_calib_data.dig_P5)) << 1);
    var2 = (var2 >> 2) + (((int32_t)_calib_data.dig_P4) << 16);
    var1 = (((_calib_data.dig_P3 * (((var1 >> 2) * (var1 >> 2)) >> 13)) >> 3) + ((((int32_t)_calib_data.dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1)) * ((int32_t)_calib_data.dig_P1)) >> 15);
    if (var1 == 0)
    {
        return 0;
        // avoid exception caused by division by zero
    }
    p = (((uint32_t)(((int32_t)1048576) - uncompensatedPress) - (var2 >> 12))) * 3125;
    if (p < 0x80000000)
    {
        p = (p << 1) / ((uint32_t)var1);
    }
    else
    {
        p = (p / (uint32_t)var1) * 2;
    }
    var1 = (((int32_t)_calib_data.dig_P9) * ((int32_t)(((p >> 3) * (p >> 3)) >> 13))) >> 12;
    var2 = (((int32_t)(p >> 2)) * ((int32_t)_calib_data.dig_P8)) >> 13;
    p = (uint32_t)((int32_t)p + ((var1 + var2 + _calib_data.dig_P7) >> 4));
    return p;
}