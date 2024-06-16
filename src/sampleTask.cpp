#include <bmp280.h>
#include <math.h>

#include "ArduinoJson.h"
#include "cmdlib.h"
#include "include.h"
#define SEA_LEVEL_PRESSURE 1013.25

static const char *TAG = "sampleTask";

TaskHandle_t xSampleTaskHandle = NULL;

void vSampleTask(void *pvParameters) {
    // Setup SPI
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    // Setup SPI Device: BMP280
    spi_device_interface_config_t devcfg = {
        .mode = 0,                   // SPI mode 0
        .clock_speed_hz = 1000000,   // Clock out a 1 MHz
        .spics_io_num = PIN_NUM_CS,  // CS pin
        .queue_size = 7,             // We want to be able to queue 7 transactions at a time
    };

    BMP280 bmp280 = BMP280();

    esp_err_t ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    if (ret != ESP_OK) {
        // terminar ejecucion de esta tarea
        ESP_LOGE(TAG, "Se aborto la ejecucion de la tarea");
        // Terminar tarea
        vTaskDelete(NULL);
    }

    ret = spi_bus_add_device(VSPI_HOST, &devcfg, bmp280.getSPIHandle());
    if (ret != ESP_OK) {
        // terminar ejecucion de esta tarea
        ESP_LOGE(TAG, "Se aborto la ejecucion de la tarea");
        // Terminar tarea
        vTaskDelete(NULL);
    }

    ret = bmp280.init(BMP280::Mode::DYNAMIC);
    if (ret != ESP_OK) {
        // terminar ejecucion de esta tarea
        ESP_LOGE(TAG, "Se aborto la ejecucion de la tarea");
        // Terminar tarea
        vTaskDelete(NULL);
    }

    Cmd *cmd;
    JsonDocument doc;
    char buffer[256];
    size_t n;

    ESP_LOGI(TAG, "SampleTask started.");

    while (1) {
        ret = bmp280.read();
        if (ret != ESP_OK) {
            cmd = new Cmd(0x02, 0, nullptr, 0x00);
            // add Cmd to queue
            xQueueSendToBack(xUploadQueue, (const void *)cmd, portMAX_DELAY);
        } else {
            doc["temp"] = bmp280.getTemp();
            doc["pres"] = bmp280.getPress();
            n = serializeMsgPack(doc, buffer);
            cmd = new Cmd(0x01, n, (uint8_t *) buffer, 0x00);
            // add Cmd to queue
            xQueueSendToBack(xUploadQueue, (const void *)cmd, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}