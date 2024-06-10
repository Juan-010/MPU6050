#include "include.h"
#include <bmp280.h>


TaskHandle_t xSampleTaskHandle = NULL;

void vSampleTask( void *pvParameters )
{
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    
    BMP280 bmp280 = BMP280();

    // Setup SPI
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    esp_err_t ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);

    // Setup SPI Device: BMP280
    spi_device_interface_config_t devcfg = {
        .mode = 0,                  // SPI mode 0
        .clock_speed_hz = 1000000,  // Clock out a 1 MHz
        .spics_io_num = PIN_NUM_CS, // CS pin
        .queue_size = 7,            // We want to be able to queue 7 transactions at a time
    };
    ret = spi_bus_add_device(VSPI_HOST, &devcfg, bmp280.getSPIHandle());
    ESP_ERROR_CHECK(ret);

    bmp280.init(BMP280::Mode::DYNAMIC);

    while (1)
    {
        bmp280.read();
        printf("Pressure: %.2f, Temperature: %.2f\n", bmp280.getPressF(), bmp280.getTempF());
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}