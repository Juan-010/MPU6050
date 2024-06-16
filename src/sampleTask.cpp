#include "include.h"
#include <bmp280.h>
#include <math.h>
#define SEA_LEVEL_PRESSURE 1013.25

static const char *TAG = "sampleTask";

TaskHandle_t xSampleTaskHandle = NULL;

void vSampleTask( void *pvParameters )
{ 
    BMP280 bmp280 = BMP280();

    // Setup SPI Device: BMP280
    spi_device_interface_config_t devcfg = {
        .mode = 0,                  // SPI mode 0
        .clock_speed_hz = 1000000,  // Clock out a 1 MHz
        .spics_io_num = PIN_NUM_CS, // CS pin
        .queue_size = 7,            // We want to be able to queue 7 transactions at a time
    };
    esp_err_t ret = spi_bus_add_device(VSPI_HOST, &devcfg, bmp280.getSPIHandle());
    ESP_ERROR_CHECK(ret);

    bmp280.init(BMP280::Mode::DYNAMIC);

    ESP_LOGI(TAG, "SampleTask started.");
    while (1)
    {   
        ret = bmp280.read();
        if (ret != ESP_OK)
        {
            printf("Error leyendo el dispositivo.\n");
        }
        else
        {
            printf("Pressure: %.2f hPA, Temperature: %.2f C\n", bmp280.getPressF(), bmp280.getTempF());
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}