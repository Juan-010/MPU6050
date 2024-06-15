#include "include.h"

extern "C" void app_main(void) {
    esp_err_t ret;

    // Setup UART
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);

    // Setup SPI
    spi_bus_config_t buscfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = PIN_NUM_MISO,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    ret = spi_bus_initialize(VSPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);


    // Limpiar el buffer del UART
    uart_flush(UART_NUM_0);

    printf("Peron vive satifactoriamente\n");

    // Crear tarea
    BaseType_t result = xTaskCreate(vSampleTask, "SampleTask", 2048, NULL, tskIDLE_PRIORITY, &xSampleTaskHandle);
    if (result != pdPASS) {
        ESP_LOGE("app_main", "Error al crear la tarea: SampleTask");
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}