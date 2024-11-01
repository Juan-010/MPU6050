#ifndef TRAMA_H
#define TRAMA_H

#include <stdint.h>

//Trama de datos
#define TRAMA_ID 0x1b
typedef struct {
    uint8_t id;
    uint8_t ctr;
    int16_t acce[3];
    int16_t gyro[3];
} Trama_t;

#endif