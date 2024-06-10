/**
 * @file calibration.cpp
 * @brief BMP280 calibration data functions implementation file.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#include "../include/bmp280.h"
#include "../include/calibration.h"
#include "../include/registers.h"

// Función para leer datos de calibración
void BMP280::readCalibrationData()
{
    uint8_t calib[24];
    readReg(BMP280_REG_CALIB00, calib, 24);

    _calib_data.dig_T1 = (calib[1] << 8) | calib[0];
    _calib_data.dig_T2 = (calib[3] << 8) | calib[2];
    _calib_data.dig_T3 = (calib[5] << 8) | calib[4];
    _calib_data.dig_P1 = (calib[7] << 8) | calib[6];
    _calib_data.dig_P2 = (calib[9] << 8) | calib[8];
    _calib_data.dig_P3 = (calib[11] << 8) | calib[10];
    _calib_data.dig_P4 = (calib[13] << 8) | calib[12];
    _calib_data.dig_P5 = (calib[15] << 8) | calib[14];
    _calib_data.dig_P6 = (calib[17] << 8) | calib[16];
    _calib_data.dig_P7 = (calib[19] << 8) | calib[18];
    _calib_data.dig_P8 = (calib[21] << 8) | calib[20];
    _calib_data.dig_P9 = (calib[23] << 8) | calib[22];    
}