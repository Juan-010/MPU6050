/**
 * @file spi.cpp
 * @brief BMP280 SPI communication functions implementation file.
 * @author Enrique Walter Philippeaux & Juan Ignacio Sanchez Serantes
 * @date 2024-06-10
 */

#include <string.h>

#include "../include/bmp280.h"

const uint8_t bmp280_mode_values[6][2] = {
    {BMP280_CTRL_MEAS_LOW_POWER, BMP280_REG_CONFIG_LOW_POWER},
    {BMP280_CTRL_MEAS_DYNAMIC, BMP280_REG_CONFIG_DYNAMIC},
    {BMP280_CTRL_MEAS_WEATHER, BMP280_REG_CONFIG_WEATHER},
    {BMP280_CTRL_MEAS_ELEVATOR, BMP280_REG_CONFIG_ELEVATOR},
    {BMP280_CTRL_MEAS_DROP, BMP280_REG_CONFIG_DROP},
    {BMP280_CTRL_MEAS_NAVIGATION, BMP280_REG_CONFIG_NAVIGATION}};

// Función para escribir un registro de 8 bits del BMP280
esp_err_t BMP280::writeReg(uint8_t reg, uint8_t value) {
    return writeReg(reg, &value, 1);
}

// Función para escribir un registro de 8 bits del BMP280
esp_err_t BMP280::writeReg(uint8_t reg, uint8_t *buffer, uint8_t n) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    uint8_t *tx_buffer = new uint8_t[n + 1];
    tx_buffer[0] = reg & 0x7F;  // Dirección con el bit de escritura
    t.length = 8 + n * 8;
    t.tx_buffer = tx_buffer;
    esp_err_t ret = spi_device_transmit(_spi, &t);  // Transmitir
    delete[] tx_buffer;
    return ret;
}

esp_err_t BMP280::setMode(Mode mode) {
    _mode = mode;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    static uint8_t tx_buffer[4];
    tx_buffer[0] = BMP280_REG_CTRL_MEAS & 0x7F;  // Dirección con el bit de escritura
    tx_buffer[1] = bmp280_mode_values[(uint8_t)mode][0];
    tx_buffer[2] = BMP280_REG_CONFIG & 0x7F;  // Dirección con el bit de escritura
    tx_buffer[3] = bmp280_mode_values[(uint8_t)mode][1];
    t.length = 4 * 8;
    t.tx_buffer = tx_buffer;
    return spi_device_transmit(_spi, &t);  // Transmitir;
}

// Función para leer un registro de 8 bits del BMP280
uint8_t BMP280::readReg(uint8_t reg) {
    uint8_t value;
    readReg(reg, &value, 1);
    return value;
}

esp_err_t BMP280::readValues() {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    const uint8_t reg = BMP280_REG_PRESS_MSB | 0x80;  // Dirección con el bit de lectura
    t.length = 7 * 8;                                 // n*8 bits de datos
    t.tx_buffer = &reg;
    t.rx_buffer = __read_buffer;
    esp_err_t ret = spi_device_transmit(_spi, &t);  // Transmitir
    return ret;
}

uint8_t BMP280::readReg(uint8_t reg, uint8_t *buffer, uint8_t n) {
    spi_transaction_t t;
    uint8_t *tmpbuffer = new uint8_t[n + 1];
    memset(&t, 0, sizeof(t));
    reg |= 0x80;           // Dirección con el bit de lectura
    t.length = 8 + n * 8;  // n*8 bits de datos
    t.tx_buffer = &reg;
    t.rx_buffer = tmpbuffer;
    esp_err_t ret = spi_device_transmit(_spi, &t);  // Transmitir
    memcpy(buffer, tmpbuffer + 1, n);
    delete[] tmpbuffer;
    return ret;
}
