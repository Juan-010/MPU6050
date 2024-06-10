/**
 * @file spi.cpp
 * @brief BMP280 SPI communication functions implementation file.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#include "../include/bmp280.h"
#include <string.h>

// Función para escribir un registro de 8 bits del BMP280
esp_err_t BMP280::writeReg(uint8_t reg, uint8_t value)
{
    return writeReg(reg, &value, 1);
}


// Función para escribir un registro de 8 bits del BMP280
esp_err_t BMP280::writeReg(uint8_t reg, uint8_t *buffer, uint8_t n)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t *tx_buffer = new uint8_t[n + 1];
    tx_buffer[0] = reg & 0x7F; // Dirección con el bit de escritura
    t.length = 8 + n*8;
    t.tx_buffer = tx_buffer;
    esp_err_t ret = spi_device_transmit(_spi, &t); // Transmitir
    delete[] tx_buffer;
    return ret;
}

// Función para leer un registro de 8 bits del BMP280
uint8_t BMP280::readReg(uint8_t reg)
{
    uint8_t value;
    readReg(reg, &value, 1);
    return value;
}

uint8_t BMP280::readReg(uint8_t reg, uint8_t *buffer, uint8_t n)
{
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    reg |= 0x80; // Dirección con el bit de lectura
    t.length = 8;                        // 8 bits de dirección y 8 bits de datos
    t.tx_buffer = &reg;
    t.rx_buffer = buffer;
    
    esp_err_t ret = spi_device_transmit(_spi, &t); // Transmitir

    return ret;
}
