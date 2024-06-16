#include "include.h"
#include "cmdlib.h"

static const char *TAG = "uploadTask";

TaskHandle_t xUploadTaskHandle = NULL;

//create queue to send data to this task.
QueueHandle_t xUploadQueue = NULL;

void vUploadTask( void *pvParameters) 
{

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
    ret = uart_param_config(UART_NUM_0, &uart_config);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART configured.");
    ret = uart_set_pin(UART_NUM_0, TX_PIN, RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART pins set.");
    ret = uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI(TAG, "UART driver installed.");
    // Limpiar el buffer del UART
    uart_flush(UART_NUM_0);

    xUploadQueue = xQueueCreate(16, sizeof(void *));
    if (xUploadQueue == NULL)
    {
        ESP_LOGE(TAG, "Error al crear la cola.");
    }

    void *data;
    
    Cmd *cmd;
    uint8_t *txBuffer;
    uint16_t len;
    while (1)
    {
        if (xQueueReceive(xUploadQueue, &data, portMAX_DELAY) == pdTRUE)
        {
            cmd = (Cmd *)data;
            cmd->serialize(txBuffer, len);
            uart_write_bytes(UART_NUM_0, (const void *)txBuffer, len);
            delete[] txBuffer;
            cmd->~Cmd();
        }
    }
}