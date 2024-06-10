#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "bmp280.h"

spi_device_handle_t spi;

// Definiciones de pines SPI
#define PIN_NUM_MISO 19
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  18
#define PIN_NUM_CS   5

// Definiciones de pines UART
#define TX_PIN (UART_PIN_NO_CHANGE)
#define RX_PIN (UART_PIN_NO_CHANGE)

float ReadTemp(void);
float ReadPress(void);
void read_mode_from_uart(int *mode);

void app_main(void) {
    esp_err_t ret;

    // Configurar parámetros del puerto UART
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };

    // Configurar los pines del puerto UART
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Inicializar el puerto UART
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);

    // Configurar el bus SPI
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    // Configurar el dispositivo SPI
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 1000000,  // Clock out a 1 MHz
        .mode = 0,                  // SPI mode 0
        .spics_io_num = PIN_NUM_CS, // CS pin
        .queue_size = 7,            // We want to be able to queue 7 transactions at a time
    };

    // Inicializar el bus SPI
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);

    // Agregar el dispositivo al bus
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    // Limpiar el buffer del UART
    uart_flush(UART_NUM_0);
    //Inicio de la interfaz de usuario
    printf("\n\n\n\n\n");
    printf("Bienvenido al programa sobre el uso del BMP280:\n\n\n");
    // Solicitar al usuario que seleccione el modo
    printf("Modos de uso del BMP280:\n");
    printf("0. Dispositivo portatil de baja potencia\n");
    printf("1. Dispositivo portatil dinamico\n");
    printf("2. Monitor de clima\n");
    printf("3. Modo Elevador/piso\n");
    printf("4. Deteccion de caida\n");
    printf("5. Navegacion interior\n");
    printf("Selecciona el modo de uso del BMP280 que deseas: \n");

    // Limpiar el buffer del UART
    uart_flush(UART_NUM_0);

    int mode = 0;
    read_mode_from_uart(&mode);

    // Inicializar el BMP280
    bmp280_init(mode);
    

    while (1) {
        float temp, press, LocalAltitude;

        
        temp = ReadTemp();
        press = ReadPress();

        // Calcular altitud (ajustar sea_level_pressure según condiciones locales)
        LocalAltitude = calculate_altitude(press, 101300.0);  // Usa 101300 Pa como presión a nivel del mar

        char uart_buffer[100]; // Buffer para los datos a enviar
        sprintf(uart_buffer, "Temperatura: %.2f C, Presion: %.2f hPa, Altura local: %.2f m  \r\n", temp, press/100, LocalAltitude);
        uart_write_bytes(UART_NUM_0, uart_buffer, strlen(uart_buffer));
       
        vTaskDelay(pdMS_TO_TICKS(3000)); // Espera 3 segundos
    }
}

float ReadTemp(){
    uint8_t msb, lsb, xlsb;
    int32_t adc_T;
    float temp;

    // Leer datos de temperatura
    msb = bmp280_spi_read(BMP280_REG_TEMP_MSB);
    lsb = bmp280_spi_read(BMP280_REG_TEMP_LSB);
    xlsb = bmp280_spi_read(BMP280_REG_TEMP_XLSB);
    adc_T = (msb << 12) | (lsb << 4) | (xlsb >> 4);

    // Compensar lecturas
    temp = bmp280_compensate_T(adc_T) / 100.0;

    return temp;
}

float ReadPress(){
    uint8_t msb, lsb, xlsb;
    int32_t adc_P;
    float press;

    // Leer datos de presión
    msb = bmp280_spi_read(BMP280_REG_PRESS_MSB);
    lsb = bmp280_spi_read(BMP280_REG_PRESS_LSB);
    xlsb = bmp280_spi_read(BMP280_REG_PRESS_XLSB);
    adc_P = (msb << 12) | (lsb << 4) | (xlsb >> 4);

    // Compensar lecturas
    press = bmp280_compensate_P(adc_P) / 256.0;

    return press;
}

void read_mode_from_uart(int *mode) {
    char buf[2] = {0}; // Buffer para almacenar la entrada del usuario
    uart_read_bytes(UART_NUM_0, (uint8_t *)buf, 1, portMAX_DELAY); // Leer un byte del UART
    *mode = buf[0] - '0'; // Convertir el carácter a un entero
}
