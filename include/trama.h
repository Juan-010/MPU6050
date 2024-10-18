#ifndef TRAMA_H
#define TRAMA_H

#include <stdint.h>
#include "mpu6050.h"

//Trama de datos
#define TRAMA_ID 0x1b
typedef struct {
    uint8_t id;
    uint8_t ctr;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    mpu6050_temp_value_t temp;
} Trama_t;

#endif