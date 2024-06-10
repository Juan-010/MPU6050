#include "bmp280.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"


static const char *TAG = "BMP280_SPI";
int32_t t_fine; //variable global para la compensación de temperatura y presión


// Estructura para almacenar los datos de calibración
struct bmp280_calib_param calib_data;

// Función para escribir en un registro del BMP280
void bmp280_spi_write(uint8_t reg, uint8_t value) {
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    uint8_t tx_data[2] = {reg & 0x7F, value};  // Dirección y valor (reg & 0x7F para el bit de escritura)
    t.length = 16;  // 8 bits de dirección y 8 bits de datos
    t.tx_buffer = tx_data;
    spi_device_transmit(spi, &t);  // Transmitir
}

// Función para leer de un registro del BMP280
uint8_t bmp280_spi_read(uint8_t reg) {
    spi_transaction_t t;
    uint8_t rx_data[2] = {0};

    memset(&t, 0, sizeof(t));
    uint8_t tx_data[2] = {reg | 0x80, 0};  // Dirección con el bit de lectura
    t.length = 16;  // 8 bits de dirección y 8 bits de datos
    t.tx_buffer = tx_data;
    t.rx_buffer = rx_data;
    spi_device_transmit(spi, &t);  // Transmitir

    return rx_data[1];  // Devolver solo el byte de datos
}

// Función para leer datos de calibración
void bmp280_read_calibration_data() {
    uint8_t calib[24];
    for (int i = 0; i < 24; ++i) {
        calib[i] = bmp280_spi_read(BMP280_REG_CALIB00 + i);
    }

    calib_data.dig_T1 = (calib[1] << 8) | calib[0];
    calib_data.dig_T2 = (calib[3] << 8) | calib[2];
    calib_data.dig_T3 = (calib[5] << 8) | calib[4];
    calib_data.dig_P1 = (calib[7] << 8) | calib[6];
    calib_data.dig_P2 = (calib[9] << 8) | calib[8];
    calib_data.dig_P3 = (calib[11] << 8) | calib[10];
    calib_data.dig_P4 = (calib[13] << 8) | calib[12];
    calib_data.dig_P5 = (calib[15] << 8) | calib[14];
    calib_data.dig_P6 = (calib[17] << 8) | calib[16];
    calib_data.dig_P7 = (calib[19] << 8) | calib[18];
    calib_data.dig_P8 = (calib[21] << 8) | calib[20];
    calib_data.dig_P9 = (calib[23] << 8) | calib[22];
}

void bmp280_init(int mode) {
    uint8_t id = bmp280_spi_read(BMP280_REG_ID);
    ESP_LOGI(TAG, "BMP280 ID: 0x%x", id);

    if (id != 0x58) {
        ESP_LOGE(TAG, "No se pudo encontrar un sensor BMP280 válido, verificar la conexión!");
        while (1) { vTaskDelay(1); }
    }

    // Leer datos de calibración
    bmp280_read_calibration_data();

    // Resetear el sensor
    bmp280_spi_write(BMP280_REG_RESET, 0xB6);
    vTaskDelay(pdMS_TO_TICKS(100));

    switch(mode){
        case 0:
            printf("Modo de uso Baja Potencia seleccionado\n");
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_LOW_POTENCY);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_LOW_POTENCY);     
            break;
        case 1:
            printf("Modo de uso Dinamico seleccionado\n");
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_DINAMYC);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_DINAMYC);     
            break;
        case 2:
            printf("Modo de uso Monitor de Clima seleccionado\n"); 
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_WEATHER);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_WEATHER);     
            break;
        case 3:
            printf("Modo de uso Elevador/Piso seleccionado\n"); 
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_ELEVATOR);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_ELEVATOR);
            break;
        case 4:
            printf("Modo de uso Detección de Caída seleccionado\n"); 
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_DROP);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_DROP);
            break;
        case 5: 
            printf("Modo de uso Navegacion Interior seleccionado\n");
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_NAVIGATION);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_NAVIGATION);
            break;
        default:
            printf("Modo de uso Navegacion Interior seleccionado por defecto\n");
            bmp280_spi_write(BMP280_REG_CTRL_MEAS, BMP280_CTRL_MEAS_NAVIGATION);  
            bmp280_spi_write(BMP280_REG_CONFIG, BMP280_REG_CONFIG_NAVIGATION);
            break;
    }
    
    printf("BMP280 inicializado correctamente.\n");
}

// Función para compensar la lectura de temperatura
int32_t bmp280_compensate_T(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)calib_data.dig_T1 << 1))) *
           ((int32_t)calib_data.dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)calib_data.dig_T1)) *
             ((adc_T >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) *
           ((int32_t)calib_data.dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

// Función para compensar la lectura de presión
uint32_t bmp280_compensate_P(int32_t adc_P) {
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib_data.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib_data.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib_data.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib_data.dig_P3) >> 8) +
           ((var1 * (int64_t)calib_data.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_data.dig_P1) >> 33;
    if (var1 == 0) {
        return 0; // Evitar división por cero
    }
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib_data.dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calib_data.dig_P7) << 4);
    return (uint32_t)p;
}

// Función para calcular la altitud sobre el nivel de referencia inicial
float calculate_altitude(float pressure, float init_level_pressure) {
    return 44330.0 * (1.0 - pow(pressure / init_level_pressure, 0.1903));
}
