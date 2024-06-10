// bmp280.h
#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include "driver/spi_master.h"

//Dirección de los registros del BMP280
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

/*Modos de uso del BMP280*/
//Dispositivo portátil de baja potencia 
#define BMP280_CTRL_MEAS_LOW_POTENCY  0x5F  // Modo normal, oversampling de temp x2, oversampling de presión x16
#define BMP280_REG_CONFIG_LOW_POTENCY  0x28  // stanby time 62.5ms y filtro IIR x4
//Dispositivo portátil dinámico
#define BMP280_CTRL_MEAS_DINAMYC  0x2F  // Modo normal, oversampling de temp x1, oversampling de presión x4
#define BMP280_REG_CONFIG_DINAMYC  0x10  // stanby time 0.5ms y filtro IIR x16
//Monitor de clima
#define BMP280_CTRL_MEAS_WEATHER  0x25  // Modo normal, oversampling de temp x1, oversampling de presión x1
#define BMP280_REG_CONFIG_WEATHER  0xA0  // stanby time 1min y filtro IIR off
//Modo Elevador/piso 
#define BMP280_CTRL_MEAS_ELEVATOR  0x2F  // Modo normal, oversampling de temp x1, oversampling de presión x4
#define BMP280_REG_CONFIG_ELEVATOR  0x48  // stanby time 125ms y filtro IIR x4
//Detección de caída
#define BMP280_CTRL_MEAS_DROP  0x2B  // Modo normal, oversampling de temp x1, oversampling de presión x2
#define BMP280_REG_CONFIG_DROP  0x00  // stanby time 0.5ms y filtro IIR off
//Navegación interior
#define BMP280_CTRL_MEAS_NAVIGATION  0x5F  // Modo normal, oversampling de temp x2, oversampling de presión x16
#define BMP280_REG_CONFIG_NAVIGATION  0x10  // stanby time 0.5ms y filtro IIR x16

struct bmp280_calib_param {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;
};

extern spi_device_handle_t spi;  // Declaración de la variable spi

void bmp280_spi_write(uint8_t reg, uint8_t value);
uint8_t bmp280_spi_read(uint8_t reg);
void bmp280_read_calibration_data(void);
void bmp280_init(int mode);
int32_t bmp280_compensate_T(int32_t adc_T);
uint32_t bmp280_compensate_P(int32_t adc_P);
float calculate_altitude(float pressure, float init_level_pressure);

#endif // BMP280_H
