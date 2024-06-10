/**
 * @file registers.h
 * @brief BMP280 registers definitions.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#ifndef REGISTERS_H
#define REGISTERS_H

//Registros del BMP280
#define BMP280_REG_TEMP_XLSB 0xFC
#define BMP280_REG_TEMP_LSB  0xFB
#define BMP280_REG_TEMP_MSB  0xFA
#define BMP280_REG_PRESS_XLSB 0xF9
#define BMP280_REG_PRESS_LSB  0xF8
#define BMP280_REG_PRESS_MSB  0xF7
#define BMP280_REG_CONFIG     0xF5
#define BMP280_REG_CTRL_MEAS  0xF4
#define BMP280_REG_STATUS     0xF3
#define BMP280_REG_RESET      0xE0
#define BMP280_REG_ID         0xD0
#define BMP280_REG_CALIB00    0x88

/*BMP280 Use cases*/
// Low power portable device
#define BMP280_CTRL_MEAS_LOW_POWER  0x5F  // Normal mode, x2 temp OS, x16 press OS
#define BMP280_REG_CONFIG_LOW_POWER  0x28  // 62.5ms Standby time, x4 IIR
// Dynamic portable device
#define BMP280_CTRL_MEAS_DYNAMIC  0x2F  // Normal mode, x1 temp OS, x4 press OS
#define BMP280_REG_CONFIG_DYNAMIC  0x10  // 0.5ms Standby time, x16 IIR
// Weather station
#define BMP280_CTRL_MEAS_WEATHER  0x25  // Forced mode, x1 temp OS, x1 press OS
#define BMP280_REG_CONFIG_WEATHER  0xA0  // 1min Standby time, IIR off
// Floor change detection
#define BMP280_CTRL_MEAS_ELEVATOR  0x2F  // Normal mode, x1 temp OS, x4 press OS
#define BMP280_REG_CONFIG_ELEVATOR  0x48  // 125ms Standby time, x4 IIR 
// Drop detection
#define BMP280_CTRL_MEAS_DROP  0x2B  // Normal mode, x1 temp OS, x2 press OS
#define BMP280_REG_CONFIG_DROP  0x00  // 0.5ms Standby time, IIR off
// Indoor navigation
#define BMP280_CTRL_MEAS_NAVIGATION  0x5F  // Normal mode, x2 temp OS, x16 press OS
#define BMP280_REG_CONFIG_NAVIGATION  0x10  //0.5ms Standby time, x16 IIR


#endif